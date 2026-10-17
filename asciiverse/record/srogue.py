"""srogue preview: Super-Rogue 9.0 exploring and fighting.

Super-Rogue shows two status lines (rows 22 and 23 of its field), asks
"-- More --" and has monsters in both cases; otherwise the shared
Explorer (rlbot.py) plays it as it plays Rogue: items, doors, corridors,
fights, rest when hurt, and after about 25 seconds the stairs. After
about 48 seconds it quits (Q, y), passing the inventory and the score
pages, and the cast ends on the top adventurers. A take where the hero
dies is recorded again; of up to six takes it keeps the one that fought
and went down the most.

    python3 asciiverse/record/srogue.py
"""
import os, shutil, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rec, rlbot


def take():
    """One recording; how good it is (blows, and levels gone down), or -1."""
    r = rec.Rec("srogue", "Super-Rogue")
    r.wait_for("Level:", 8)
    r.pump(1.0)
    bot = rlbot.Explorer(r, monster="alpha", status_row=22)
    while r.elapsed() < 48:
        if "REST IN" in r.text() or "Top Ten" in r.text():
            r.finish(b"\r")
            return -1
        bot.act(descend=r.elapsed() > 20)
    r.pump(1.0)
    r.key("Q", 0.8)
    r.key("y", 1.2)
    r.key("\r", 1.2)
    r.key("\r", 1.5)
    r.finish(b"\r")
    print(f"blows {bot.blows}, levels {bot.levels}", file=sys.stderr)
    return bot.blows + 5 * bot.levels


cast = os.path.join(rec.ROOT, "asciiverse", "casts", "srogue.cast")
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
