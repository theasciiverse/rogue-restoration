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
once it is time to go down, the stairs. It steps diagonally too, where
both cells beside the step are open and neither end is a doorway, as the
games allow. When
nothing is left to explore it searches for hidden doors, and takes the
stairs if it sees them.
"""
import collections, random, re

OY, OX = 3, 10          # where the centred 80x24 field sits in 100x30
STEPS = {(0, -1): "h", (1, 0): "j", (-1, 0): "k", (0, 1): "l"}
DIAG = {(-1, -1): "y", (-1, 1): "u", (1, -1): "b", (1, 1): "n"}


class Explorer:
    def __init__(self, r, items="*!?:)]/=,", walk=".#+%", stairs="%",
                 monster=str.isupper, more=("--More--", "-more-", "--more--", "-- More --"),
                 status_row=23, hp=r"Hpt?: *(\d+)\( *(\d+)\)", rest="s",
                 down=">", top=1, bottom=22, oy=OY, ox=OX, cancel="\x1b", pickup=None,
                 pile="*"):
        self.r = r
        self.items = set(items)
        self.walk = set(walk) | self.items
        self.stairs = stairs
        # "alpha": monsters are upper and lower case letters
        self.monster = (lambda c: c.isalpha() and c != "@") if monster == "alpha" else monster
        self.more = more
        self.status_row = status_row
        self.hp = re.compile(hp)
        self.rest = rest
        self.down = down
        self.top, self.bottom = top, bottom
        self.oy, self.ox = oy, ox
        self.visited = collections.Counter()
        self.searches = 0
        self.levels = 0         # how many levels down from the first
        self.last = None
        self.stairs_at = None   # the hero hides the stairs when on them
        self.stuck_diag = None  # a diagonal blow that did nothing
        self.waits = 0          # turns waited for it
        self.escaped = None     # the last question answered with cancel
        # the answer to a question (Escape, or "n"), and to "which of
        # these to pick up (* for all)" ("*", with Return where the game
        # reads a line)
        self.cancel = cancel
        self.pile = pile
        # items it has stood on, which it no longer walks to, and the key
        # that picks one up where the game does not do it by walking on
        self.done_items = set()
        self.pickup = pickup
        self.prev = None
        self.blows = 0          # fights that showed a hit or a miss
        self.first_level = None

    def grid(self):
        rows = self.r.rows()
        return [rows[self.oy + y][self.ox:self.ox + 80] for y in range(24)]

    def find(self, g, ch):
        for y in range(self.top, self.bottom + 1):
            x = g[y].find(ch)
            if x >= 0:
                return y, x
        return None

    def inside(self, g, y, x):
        """A blank cell walled in on four sides: the floor of a room seen
        before and no longer shown (Advanced Rogue forgets dark rooms)."""
        def first(cells):
            return next((c for c in cells if c != " "), None)
        row = g[y]
        col = [g[i][x] for i in range(len(g))]
        return (first(reversed(row[:x])) in ("|", "+") and first(row[x + 1:]) in ("|", "+")
                and first(reversed(col[self.top:y])) in ("-", "+")
                and first(col[y + 1:self.bottom + 1]) in ("-", "+"))

    def path_to(self, g, start, goal, through_rooms=False):
        """First step of a shortest orthogonal path to a cell where goal(y, x)."""
        seen = {start: None}
        q = collections.deque([start])
        while q:
            y, x = q.popleft()
            if (y, x) != start and goal(y, x):
                while seen[(y, x)] != start:
                    y, x = seen[(y, x)]
                return (y - start[0], x - start[1])
            for dy, dx in list(STEPS) + list(DIAG):
                ny, nx = y + dy, x + dx
                if dy and dx and (g[y][x] == "+" or not (self.top <= ny <= self.bottom and 0 <= nx < 80)
                                  or g[ny][nx] == "+" or g[y][nx] not in self.walk
                                  or g[ny][x] not in self.walk):
                    continue    # diagonally only across open ground, not doorways
                if (self.top <= ny <= self.bottom and 0 <= nx < 80
                        and (ny, nx) not in seen
                        and (g[ny][nx] in self.walk or (ny, nx) == self.stairs_at
                             or (through_rooms and g[ny][nx] == " " and self.inside(g, ny, nx)))):
                    seen[(ny, nx)] = (y, x)
                    q.append((ny, nx))
        return None

    def hit_points(self, g):
        m = self.hp.search(g[self.status_row])
        return (int(m.group(1)), int(m.group(2))) if m else (1, 1)

    def level(self, g):
        for row in g[self.status_row - 1:self.status_row + 1]:
            m = re.search(r"(?:Level|Lvl): *(\d+)", row)
            if m:
                return int(m.group(1))
        return None

    def has_more(self, g):
        return any(m in line for line in g[:3] for m in self.more)

    def act(self, descend=False):
        """One move; returns what it did."""
        r, g = self.r, self.grid()
        if self.has_more(g):
            r.key(" ", 0.15)
            return "more"
        top = " ".join(g[:3])
        if "* for all" in top and top != self.escaped:
            r.key(self.pile, 0.3)   # which of a pile to pick up: all
            self.escaped = top      # once: the game may leave it on screen
            return "pick up all"
        if g[0].rstrip().endswith("?") and top != self.escaped:
            r.key(self.cancel, 0.2)  # any other question: no
            self.escaped = top
            return "escape"
        lv = self.level(g)
        if lv is not None:
            if self.first_level is None:
                self.first_level = lv
            if lv - self.first_level > self.levels:
                self.levels = lv - self.first_level
                self.visited.clear()
                self.done_items.clear()
                self.stairs_at = None
        me = self.find(g, "@")
        if me is None:
            r.key(" ", 0.2)
            return "lost"
        if me != self.last:
            self.searches = 0
        self.last = me
        prev, self.prev = self.prev, g
        if prev is not None and prev[me[0]][me[1]] in self.items and me not in self.done_items:
            self.done_items.add(me)
            if self.pickup:
                r.key(self.pickup, 0.3)
                self.escaped = None     # its question may come again
                return "pick up"
        self.visited[me] += 1
        y, x = me
        foe = next(((dy, dx) for dy in (-1, 0, 1) for dx in (-1, 0, 1)
                    if (dy or dx) and self.top <= y + dy <= self.bottom
                    and 0 <= x + dx < 80 and self.monster(g[y + dy][x + dx])), None)
        if foe:
            before = r.text()
            if foe in DIAG and self.stuck_diag == (me, foe) and self.waits < 3:
                # no diagonal step out of a doorway: let it come round
                self.waits += 1
                r.key(self.rest, 0.15)
                return "wait"
            if self.waits >= 3:
                # it does not come round: step back along the way in
                self.waits = 0
                self.stuck_diag = None
                back = next((d for d in STEPS if (y + d[0], x + d[1]) != (y + foe[0], x + foe[1])
                             and self.top <= y + d[0] <= self.bottom and 0 <= x + d[1] < 80
                             and g[y + d[0]][x + d[1]] in "#+."), None)
                if back:
                    r.key(STEPS[back], 0.15)
                    return "retreat"
            r.key(STEPS.get(foe) or DIAG[foe], 0.18)
            if foe in DIAG and r.text() == before:
                self.stuck_diag = (me, foe)
            msg = " ".join(self.grid()[:2]).lower()
            if "hit" in msg or "miss" in msg or "defeat" in msg or "kill" in msg:
                self.blows += 1
            return "fight"
        hp, hp_max = self.hit_points(g)
        if hp * 3 <= hp_max:
            r.key(self.rest, 0.03)     # quickly: nothing to watch
            return "rest"

        def frontier(fy, fx):
            near_dark = any(0 <= fx + dx < 80 and g[fy + dy][fx + dx] == " "
                            for dy, dx in STEPS if self.top <= fy + dy <= self.bottom)
            # corridors and doors with darkness beside them, and the
            # floor of a dark room where it is not yet seen
            return near_dark and g[fy][fx] in "#+." and self.visited[(fy, fx)] < 2

        def take_stairs():
            r.key(self.down, 0.5)
            self.visited.clear()
            self.done_items.clear()
            self.stairs_at = None
            return "stairs"

        seen = self.find(g, self.stairs)
        if seen:
            self.stairs_at = seen
        step = None
        if descend and me == self.stairs_at:
            return take_stairs()
        if descend and self.stairs_at:
            step = self.path_to(g, me, lambda fy, fx: (fy, fx) == self.stairs_at, True)
        if step is None:
            step = self.path_to(g, me, lambda fy, fx: g[fy][fx] in self.items
                                and (fy, fx) not in self.done_items)
        if step is None:
            step = self.path_to(g, me, lambda fy, fx: g[fy][fx] == "+" and self.visited[(fy, fx)] == 0, True)
        if step is None:
            step = self.path_to(g, me, frontier, True)
        if step is None and me == self.stairs_at:
            return take_stairs()
        if step is None and seen:
            step = self.path_to(g, me, lambda fy, fx: g[fy][fx] == self.stairs)
        if step is None:
            step = self.path_to(g, me, lambda fy, fx: self.visited[(fy, fx)] == 0)
        if step is None or self.visited[me] > 12:
            # nothing left to walk to: search for hidden doors, then wander
            self.searches += 1
            if descend and self.searches == 1:
                # the stairs may be under the hero since it arrived
                r.key(self.down, 0.5)
                return "stairs?"
            if self.searches < 8:
                r.key(self.rest, 0.12)
                return "search"
            self.visited[me] = 0
            r.key(random.choice("hjkl"), 0.12)
            return "wander"
        r.key(STEPS.get(step) or DIAG[step], 0.11)
        return "move"
