# Bugs in the original code

Places where the game's code does something the developers probably didn't mean to. They're kept in the decomp since it has to match, but a mod could fix them.

How sure we are about each one:

- **Sure**: the code can't do what it seems meant to do.
- **Likely**: probably a mistake.
- **Maybe**: could be on purpose.

## get_help_instruction ((sure))

The help script's move command moves the pointer down when it should move it right. For direction bit 1 ((right)) it does `++pointer_y` instead of `++pointer_x`. The same test a few lines earlier uses x.

## restore_life ((sure))

The half-heart step after the loop can never run. The damage was just set to 0, so the count is always even. You can't see this in the game because life is refilled completely anyway.

## check_sprite_collisions ((sure))

It checks for `result == 2` ((blocked by a shield)), but `get_collision_result` only ever returns 0 or 1, and the shield mask is passed to an argument that is never used. So enemy shields never block Link's sword through this path.

## erase_death_number ((sure))

It calls `draw_filled_rect(254, 104, 131, 131, ...)`. The right edge ((131)) is left of the left edge ((254)), so nothing is erased. You can't see this, because the new number is drawn over the old one.

## scroll_world ((sure))

It sets `map_moved = 1` and `view_moved = 1` every frame, so the two checks after it are always true. The map and the pointer are redrawn every frame, and the "pointer didn't move" recentring only works the first time.

## check_for_alternate_macro, play_bumper, play_error ((sure))

Some paths in these functions return without a value, so the caller gets whatever is left in register d0. This happens to work in practice.

## move_type_5_or_6_1_block ((sure, harmless))

It reads through pointers that are only set for kind 6. This is harmless because it is only ever called by the kind 6 platform code.

## leave_item, case 16 ((likely))

Case 16 stops without setting the item or whether it is temporary. An enemy with drop kind 16 would drop garbage if any enemy uses that kind.

## check_enemy_combat, type 4, state 3 ((likely))

It compares against `cparams[28]` ((the step per frame)), where every other place uses `cparams[26]` ((the target)). A flying enemy's dive would stop at the wrong height.

## check_enemy_combat, type 4, state 1 ((maybe))

It passes `alt_active` ((0 or 1)) as the facing direction to `check_combat_box`, so that attack box is never flipped.

## set_alternate_macro ((maybe))

When the state is neither 2 nor 6, it leaves the image offset unchanged, which could give some alternate animations the wrong facing.

## can_save_restore ((maybe))

The loop runs up to `<= mm_row_count`, one row past the last one, and reads one extra save slot flag.

## start_anim, room 63 ((maybe))

It gives the Lantern of Vision without checking for the Crystal of Vision, unlike every other trade. Could be on purpose.
