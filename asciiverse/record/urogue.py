"""urogue preview: UltraRogue, a fighter exploring and fighting.

The driver picks the fighter, does not keep it as a saved character and
leaves Friendly Fiend's Flea Market, where every game starts. UltraRogue
has monsters in both cases, the hero picks up whole piles ("*" and
Return), and ^G answers other questions, since UltraRogue reads Escape
and a key soon after as Alt and the key. Otherwise the shared Explorer (rlbot.py) plays it as it plays
Rogue: items, doors, corridors, fights, rest when hurt, and after about
20 seconds the stairs. After about 48 seconds it quits (Q, y), and the
cast ends on the top ten. A take where the hero
dies is recorded again; of up to six takes it keeps the one that fought
and went down the most.

    python3 asciiverse/record/urogue.py
"""
import os, shutil, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rec, rlbot


def take():
    """One recording; how good it is (blows, and levels gone down), or -1."""
    r = rec.Rec("urogue", "UltraRogue")
    r.wait_for("character class", 8)
    r.key("a", 0.8)
    r.key("n", 0.8)
    r.key("l", 0.8)
    r.wait_for("Lvl:", 8)
    r.pump(1.0)
    bot = rlbot.Explorer(r, monster="alpha", pile="*\r", cancel="\x07")
    while r.elapsed() < 48:
        if "REST" in r.text() or "Top Ten" in r.text():
            r.finish(b"\r")
            return -1
        bot.act(descend=r.elapsed() > 20)
    r.pump(1.0)
    for _ in range(8):          # out of any page or question first
        if "More" in r.text() or "SPACE" in r.text():
            r.key(" ", 0.4)
        elif "for all" in r.text() or r.text().split("\n")[3].rstrip().endswith("?"):
            r.key("\x07", 0.4)
        else:
            break
    r.key("Q", 0.8)
    r.key("y", 1.5)
    for _ in range(6):          # the pack, page by page, then the scores
        if "Top Ten" in r.text():
            break
        r.key("\r", 1.2)
    r.pump(1.5)
    r.finish(b"\r")
    print(f"blows {bot.blows}, levels {bot.levels}", file=sys.stderr)
    return bot.blows + 5 * bot.levels


cast = os.path.join(rec.ROOT, "asciiverse", "casts", "urogue.cast")
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
