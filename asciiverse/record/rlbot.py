"""A simple player for the Restoration Project games, shared by the drivers.

All six games draw the same kind of dungeon: rooms of . walled with - and
|, doors +, corridors #, stairs %, the hero @, monsters as letters, items
as punctuation, and a status line at the bottom. compat/centre.h centres
their 80x24 field, so in the 100x30 recording it starts at row 3,
column 10.

Explorer.act() makes one move, the way NetBSD rogue's driver does
(theasciiverse/netbsd-games, asciiverse/record/rogue.py): answer a
--More--, hit a monster beside the hero, rest when hurt, else walk to the
nearest item, a door not yet visited, the open end of a corridor, or,
once it is time to go down, the stairs. It only ever steps across or up
and down, which is always allowed; diagonals are used for fighting. When
nothing is left to explore it searches for hidden doors, and takes the
stairs if it sees them.
"""
import collections, random, re

OY, OX = 3, 10          # where the centred 80x24 field sits in 100x30
STEPS = {(0, -1): "h", (1, 0): "j", (-1, 0): "k", (0, 1): "l"}
DIAG = {(-1, -1): "y", (-1, 1): "u", (1, -1): "b", (1, 1): "n"}


class Explorer:
    def __init__(self, r, items="*!?:)]/=,", walk=".#+%", stairs="%",
                 monster=str.isupper, more=("--More--", "-more-", "--more--"),
                 status_row=23, hp=r"Hp: *(\d+)\((\d+)\)", rest="s",
                 down=">", top=1, bottom=22, oy=OY, ox=OX):
        self.r = r
        self.items = set(items)
        self.walk = set(walk) | self.items
        self.stairs = stairs
        self.monster = monster
        self.more = more
        self.status_row = status_row
        self.hp = re.compile(hp)
        self.rest = rest
        self.down = down
        self.top, self.bottom = top, bottom
        self.oy, self.ox = oy, ox
        self.visited = collections.Counter()
        self.searches = 0
        self.levels = 0
        self.last = None
        self.stairs_at = None   # the hero hides the stairs when on them
        self.stuck_diag = None  # a diagonal blow that did nothing
        self.blows = 0          # fights that showed a hit or a miss

    def grid(self):
        rows = self.r.rows()
        return [rows[self.oy + y][self.ox:self.ox + 80] for y in range(24)]

    def find(self, g, ch):
        for y in range(self.top, self.bottom + 1):
            x = g[y].find(ch)
            if x >= 0:
                return y, x
        return None

    def path_to(self, g, start, goal):
        """First step of a shortest orthogonal path to a cell where goal(y, x)."""
        seen = {start: None}
        q = collections.deque([start])
        while q:
            y, x = q.popleft()
            if (y, x) != start and goal(y, x):
                while seen[(y, x)] != start:
                    y, x = seen[(y, x)]
                return (y - start[0], x - start[1])
            for dy, dx in STEPS:
                ny, nx = y + dy, x + dx
                if (self.top <= ny <= self.bottom and 0 <= nx < 80
                        and (ny, nx) not in seen and g[ny][nx] in self.walk):
                    seen[(ny, nx)] = (y, x)
                    q.append((ny, nx))
        return None

    def hit_points(self, g):
        m = self.hp.search(g[self.status_row])
        return (int(m.group(1)), int(m.group(2))) if m else (1, 1)

    def has_more(self, g):
        return any(m in line for line in g[:3] for m in self.more)

    def act(self, descend=False):
        """One move; returns what it did."""
        r, g = self.r, self.grid()
        if self.has_more(g):
            r.key(" ", 0.15)
            return "more"
        me = self.find(g, "@")
        if me is None:
            r.key(" ", 0.2)
            return "lost"
        if me != self.last:
            self.searches = 0
        self.last = me
        self.visited[me] += 1
        y, x = me
        foe = next(((dy, dx) for dy in (-1, 0, 1) for dx in (-1, 0, 1)
                    if (dy or dx) and self.top <= y + dy <= self.bottom
                    and 0 <= x + dx < 80 and self.monster(g[y + dy][x + dx])), None)
        if foe:
            before = r.text()
            if foe in DIAG and self.stuck_diag == (me, foe):
                # no diagonal step out of a doorway: let it come round
                r.key(self.rest, 0.15)
                return "wait"
            r.key(STEPS.get(foe) or DIAG[foe], 0.18)
            if foe in DIAG and r.text() == before:
                self.stuck_diag = (me, foe)
            msg = " ".join(self.grid()[:2]).lower()
            if "hit" in msg or "miss" in msg or "defeat" in msg or "kill" in msg:
                self.blows += 1
            return "fight"
        hp, hp_max = self.hit_points(g)
        if hp * 3 <= hp_max:
            r.key(self.rest, 0.12)
            return "rest"

        def frontier(fy, fx):
            near_dark = any(0 <= fx + dx < 80 and g[fy + dy][fx + dx] == " "
                            for dy, dx in STEPS if self.top <= fy + dy <= self.bottom)
            return near_dark and g[fy][fx] in "#+" and self.visited[(fy, fx)] < 2

        def take_stairs():
            r.key(self.down, 0.5)
            self.visited.clear()
            self.stairs_at = None
            self.levels += 1
            return "stairs"

        seen = self.find(g, self.stairs)
        if seen:
            self.stairs_at = seen
        step = None
        if descend and me == self.stairs_at:
            return take_stairs()
        if descend and seen:
            step = self.path_to(g, me, lambda fy, fx: g[fy][fx] == self.stairs)
        if step is None:
            step = self.path_to(g, me, lambda fy, fx: g[fy][fx] in self.items)
        if step is None:
            step = self.path_to(g, me, lambda fy, fx: g[fy][fx] == "+" and self.visited[(fy, fx)] == 0)
        if step is None:
            step = self.path_to(g, me, frontier)
        if step is None and me == self.stairs_at:
            return take_stairs()
        if step is None and seen:
            step = self.path_to(g, me, lambda fy, fx: g[fy][fx] == self.stairs)
        if step is None:
            step = self.path_to(g, me, lambda fy, fx: self.visited[(fy, fx)] == 0)
        if step is None or self.visited[me] > 12:
            # nothing left to walk to: search for hidden doors, then wander
            self.searches += 1
            if self.searches < 8:
                r.key(self.rest, 0.12)
                return "search"
            self.visited[me] = 0
            r.key(random.choice("hjkl"), 0.12)
            return "wander"
        r.key(STEPS[step], 0.11)
        return "move"
