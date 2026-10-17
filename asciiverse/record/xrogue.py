"""xrogue preview: XRogue 8.0, a fighter leaving the shop and exploring.

The driver picks the fighter and types its attributes (Int 10, Str 18,
Wis 8, Dex 14, Con 16, Charisma the 8 left over). Every game starts on
level 0 in Friendly Fiend's Equipage, so the shared Explorer (rlbot.py)
first heads down its stairs, then plays as it plays Rogue: items, doors,
corridors, fights, rest when hurt, and after about 30 seconds the
stairs again. After about 48 seconds it quits (Q, yes), and the
cast ends on the top scores. A take where the hero
dies is recorded again; of up to six takes it keeps the one that fought
and went down the most.

    python3 asciiverse/record/xrogue.py
"""
import os, shutil, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rec, rlbot


def take():
    """One recording; how good it is (blows, and levels gone down), or -1."""
    r = rec.Rec("xrogue", "XRogue")
    r.wait_for("character class", 8)
    r.key("1", 0.8)
    r.key("101881416", 1.0)
    r.key("y", 0.8)
    r.wait_for("Lvl:", 8)
    r.pump(1.0)
    # two status lines under a field of 22 rows; "P" picks things up ("*"
    # for a pile), and "n" answers other questions ("Do you want to buy
    # it?" in the shop)
    bot = rlbot.Explorer(r, monster="alpha", bottom=21, pickup="P",
                         cancel="n")
    while r.elapsed() < 48:
        if "REST" in r.text() or "Top 20" in r.text() or "Top Ten" in r.text():
            r.finish(b"\r")
            return -1
        # out of the shop on level 0 first, later down again
        bot.act(descend=bot.levels == 0 or r.elapsed() > 30)
    r.pump(1.0)
    for _ in range(6):          # clear any --More-- first
        if "More" not in r.text():
            break
        r.key(" ", 0.4)
    r.key("Q", 0.8)
    r.key("yes\r", 1.5)        # XRogue wants the word
    for _ in range(6):          # the pack, page by page, then the scores
        if "Top 20" in r.text() or "Top Ten" in r.text():
            break
        r.key("\r", 1.2)
    r.pump(1.5)
    r.finish(b"\r")
    print(f"blows {bot.blows}, levels {bot.levels}", file=sys.stderr)
    return bot.blows + 5 * (bot.levels - 1)


cast = os.path.join(rec.ROOT, "asciiverse", "casts", "xrogue.cast")
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
