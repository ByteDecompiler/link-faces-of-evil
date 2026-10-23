# Game mechanics

Notes on how *Link: The Faces of Evil* works, taken from its code. The function names in brackets are where to look for the details.

Units used on this page:

- A **cell** is 4 pixels.
- A **frame** is one step of the game loop. The game waits for a timer signal each step ((`alm_cycle(0x1f9, 0x80000010)` in `drop_sprite`)). That is probably 16 frames a second but this hasn't been confirmed.

## Items

These are the item numbers the code uses:

- 0: Lantern. Light radius $140. Uses lamp oil.
- 1: Magic Lantern. Light radius $280. Uses lamp oil.
- 2: Lantern of Vision. Light radius $500. Needs no oil. Shows the invisible flying enemies in rooms 64 to 69.
- 3: Winged Helmet. A big jump costs 5 rubies. Link rises 3, 3, 2 and 2 cells ((10 in all)). He must be standing on solid ground.
- 4: Bell. Costs 5 rubies. Freezes "type 4" enemies for 80 frames.
- 5: Firestones. A triple shot. Each shot uses one.
- 6: Snowballs. A lobbed shot. Each shot uses one.
- 7: Bombs. Only one at a time. The fuse lasts 32 frames. It does 10 damage in a box around it, and throws 8 pieces of shrapnel.
- 8: Ropes. The rope rises 3 cells a frame. Link climbs 1 cell a frame, and 3 for the last stretch.
- 9: Power Glove. Costs 10 rubies. After a 5-frame flash, it does 200 damage to every enemy it can hurt within 16 cells left or right and 10 cells up or down.
- 10: Book of Koridai. A lobbed shot that is never used up. Only one can be in the air at a time.
- 11 and 12: Canteen ((Water of Life)), full and empty. The full canteen heals Link completely and becomes empty.
- 13 to 18: Necklace, Grapple Berries, Crystal of Vision, Crystal of Reflection, Fire Diamond and Ice Crystal. Quest items, traded for other items.
- 19: Lamp oil.
- 21: Rubies. This shares its memory with the `rubies` counter.

Functions: `init_weapon`, `light_lantern`, `big_jump_up`, `power_glove_blast`, `init_bomb`, `init_bomb_hit`, `using_rope`, `lob_shot`, `triple_shot`.

## Sword and shots

- A sword thrust does 1 damage. A thrust while standing moves Link 4 pixels forward for one frame. It doesn't do this in the air, on a slope, on a moving platform, or on a ladder or rope ((`init_sword_thrust`)).
- The sword beam fires with the level 3 sword, or with the level 2 sword only when Link's health is full. It flies 4 cells a frame ((`init_weapon`, `straight_shot`)).
- The triple shot fires one shot straight ahead at 4 cells a frame, and two at 3.5 cells a frame that drift up and down ((`triple_shot`)).
- A lobbed shot moves 2.5 cells a frame forward and starts 1.25 cells a frame upward. Gravity pulls it down by 1/4 cell each frame ((`lob_shot`)).
- Shots start 2 cells lower when Link is crouching, and 4 cells higher while he rises in a jump. If a wall is within 10 cells, Link's shots fizzle out.
- One swing hits only one item lying on the floor. A placed bomb can only be hit in the shop ((`check_tool_collision`)).

## Link

- Life is counted in half hearts. Link dies when his damage reaches twice his number of hearts. He starts with 3 hearts, plus one for each heart container he has found ((`hero_hit`, `set_life_hearts`)).
- After a hit, Link flashes and can't be hit again for 64 frames. He is knocked back 1 cell a frame ((`drop_sprite`, `handle_knockback`)).
- Link can only jump again 10 frames after his last jump, and only crouch 10 frames after standing up. After falling for 3 frames he switches to the falling pose. He falls at most 3 cells a frame ((`check_which_upward_motion`, `check_which_downward_motion`, `move_link`)).
- Getting on or off a ladder moves Link 3 cells at once. Blocks $56 and $25 slide Link right and left, at up to 2 cells a frame ((`climb`, `slide`)).
- When Link dies, the "Continue?" countdown goes from 5 to 0 ((88 frames)). Pressing a button continues in the same area. A continue is only offered for the first two deaths in an area ((`death_loop`, `drop_sprite`)).

## The shop ((room 4))

- 3 bombs cost 20 rubies, 3 ropes cost 10, and 3 lamp oils cost 5. Each count stops at 99. A bought item comes back on the counter 16 frames later. Without enough rubies, the game shows "Not Enough Rubies!" ((`sprite_hits_tool`, `buy_item`, `add_fake_*`)).
- Rubies dropped by enemies are worth 1, 5 or 10, up to 999 in total. Dropped items disappear after 80 frames, and blink during the last 32 ((`leave_item`, `draw_tools`)).

## Enemies

- Each enemy has a byte for each weapon saying whether it can be hurt by it ((`check_vulnerablility`)). In combat state 7, only the Power Glove and bombs hurt it.
- A beaten enemy loses a life and comes back from its starting state. An enemy with 0 lives in its record comes back forever. One-time enemies stay gone once their experience bit is set ((`check_for_restart`, `init_enemies`)).
- Sleeping enemies wake up after a countdown, when Link walks into their wake-up box, or after a delay that starts there ((`check_sleepers`)).
- Walking enemies pick a random turning point between where they are and their limit ((`get_random_bound`)).
- What enemies drop ((`leave_item`)): kinds 1, 2 and 3 drop 1, 5 and 10 rubies; kind 4 drops 1 or 5; kind 5 drops 5 or 10; kind 6 drops 10 one time in five, otherwise 1 or 5; kind 7 drops a Snowball; kind 8 drops a Firestone; kinds 9 to 15 drop the quest items.

## Quest and rooms

- Rewards and trades in each room ((`start_anim`)):
  - Room 17: a heart container. Later visits refill Link's life and the canteen.
  - Room 42: a heart container.
  - Room 7 ((a boss)): a heart container.
  - Room 1: the level 2 sword.
  - Room 22: the Necklace is traded for the Water of Life.
  - Room 24: the Lantern.
  - Room 20: the Grapple Berries are traded for the Power Glove.
  - Room 36: the Crystal of Reflection is traded for the shield.
  - Room 30: the Ice Crystal is traded for the Magic Lantern.
  - Room 63: the Crystal of Vision is traded for the Lantern of Vision.
  - Room 43: the Fire Diamond is traded for the level 3 sword.
- Quest items lying in rooms ((`pick_up_perm_tool`)): the Book of Koridai in room 56, the Grapple Berries and Fire Diamond in room 32, the Ice Crystal in room 34, the Winged Helmet in room 40, the Bell in room 7, the Necklace in room 19, the Crystal of Reflection in room 49 and the Crystal of Vision in room 61. Several of them make the room's exit appear straight away.
- The Water of Life spots ((item spots 40 to 44)) are never used up. Every touch heals Link fully.
- Keys are item spots 0 to 10 ((experience bits $80 to $8A)). Key $85 opens two doors, in rooms 28 and 29 ((`check_for_key`)).
- Boss rooms 7, 28, 40, 54, 60, 61, 66 and 69 play a cutscene the first time ((`start_boss_first_anim`)).
- Dark rooms: 6, 19, 32, 34, 35, 36, 39, 45, 58, 59, 60, 61, 66, 67, 68 and 69 ((`set_dark_room`)).
- In room 61, a projectile blocked by the shield bounces back and can hurt any enemy ((`check_collisions`)).

## Other

- Random numbers: `seed = (seed * 415821 + 1) mod 65536`, and the result is `seed >> 2`. The seed starts from the time of day ((`randomize`, `init_random`)).
- The screen dims after 13500 frames without input in a room, or 54000 elsewhere ((`screen_dimmer`)). After 1400 frames without input on the title menu, the demo starts ((`mmenu_loop`)).
- Saves: there are 3 slots in NVRAM. Each save needs $62 bytes of game state plus $80 bytes for the file itself ((`verify_save_space`, `save_game_to_NVR`)).
- Any button skips a cutscene ((`play_animation`)).
