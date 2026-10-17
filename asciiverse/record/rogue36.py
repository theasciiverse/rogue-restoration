"""rogue36 preview: Rogue 3.6 exploring the Dungeons of Doom and fighting.

The shared Explorer (rlbot.py) walks to items, doors and the open ends of
corridors, fights what is beside it and rests when hurt; after about 25
seconds it heads for the stairs. After about 48 seconds it quits, and
the cast ends on Rogue's list of top adventurers. Each take uses its own
dungeon (SEED, which Rogue 3.6 reads like its wizard's dungeon number);
a take where the hero dies, or meets nothing to fight, is recorded again.

    python3 asciiverse/record/rogue36.py
"""
import os, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rec, rlbot


def take(seed):
    """One recording; True when the hero lived and fought."""
    r = rec.Rec("rogue36", "Rogue 3.6", extra={"SEED": str(seed)})
    r.wait_for("Level:", 8)
    r.pump(1.0)
    bot = rlbot.Explorer(r)
    while r.elapsed() < 48:
        if "REST" in r.text() or "killed" in r.text() or "Top Ten" in r.text():
            r.finish(b"\r")
            return False
        bot.act(descend=r.elapsed() > 25)
    r.pump(1.0)
    r.key("Q", 0.8)
    r.key("y", 2.0)
    r.finish(b"")
    print(f"blows {bot.blows}, levels {bot.levels}", file=sys.stderr)
    return bot.blows >= 4


for attempt in range(12):
    if take(1000 + attempt):
        break
    print("no good take; recording again", file=sys.stderr)
