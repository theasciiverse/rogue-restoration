"""arogue preview: Advanced Rogue 5.8, a fighter exploring and fighting.

The driver picks the fighter and keeps the first roll; Advanced Rogue
has monsters in both cases and forgets the floor of rooms the hero has
left. Otherwise the shared Explorer (rlbot.py) plays it as it plays
Rogue: items, doors, corridors, fights, rest when hurt, and after about
20 seconds the stairs. After about 48 seconds it quits (Q, y), and the
cast ends on the top twenty,
after the pack. A take where the hero
dies is recorded again; of up to six takes it keeps the one that fought
and went down the most.

    python3 asciiverse/record/arogue.py
"""
import os, shutil, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rec, rlbot


def take():
    """One recording; how good it is (blows, and levels gone down), or -1."""
    r = rec.Rec("arogue", "Advanced Rogue")
    r.wait_for("character class", 8)
    r.key("1", 0.8)
    r.key("n", 0.8)
    r.wait_for("Lvl:", 8)
    r.pump(1.0)
    bot = rlbot.Explorer(r, monster="alpha")
    while r.elapsed() < 48:
        if "REST" in r.text() or "Top 20" in r.text():
            r.finish(b"\r")
            return -1
        bot.act(descend=r.elapsed() > 20)
    r.pump(1.0)
    for _ in range(6):          # clear any --More-- first
        if "More" not in r.text():
            break
        r.key(" ", 0.4)
    r.key("Q", 0.8)
    r.key("y", 1.5)
    for _ in range(6):          # the pack, page by page, then the scores
        if "Top 20" in r.text():
            break
        r.key("\r", 1.2)
    r.pump(1.5)
    r.finish(b"\r")
    print(f"blows {bot.blows}, levels {bot.levels}", file=sys.stderr)
    return bot.blows + 5 * bot.levels


cast = os.path.join(rec.ROOT, "asciiverse", "casts", "arogue.cast")
best = -1
for attempt in range(6):
    got = take()
    if got > best:
        best = got
        shutil.copy(cast, cast + ".best")
    if best >= 12:
        break
shutil.move(cast + ".best", cast)
print(f"kept a take worth {best}", file=sys.stderr)
