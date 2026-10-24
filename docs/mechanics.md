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

## The shop ((room 4, Goronu))

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
  - Room 17 ((Spearfish, the fairies)): a heart container. Later visits refill Link's life and the canteen.
  - Room 42 ((Shipwreck Cliff, Clora)): a heart container.
  - Room 7 ((Goronu, the boss Goronu)): a heart container.
  - Room 1 ((Crater Cove, Anutu)): the level 2 sword.
  - Room 22 ((Toyku Lighthouse, Alora)): the Necklace is traded for the Water of Life.
  - Room 24 ((Toyku Lighthouse, Horgam)): the Lantern.
  - Room 20 ((Spearfish, Zorga)): the Grapple Berries are traded for the Power Glove.
  - Room 36 ((Serigon Caves, Suprena)): the Crystal of Reflection is traded for the shield.
  - Room 30 ((Harlequin Bazaar, Odranoel)): the Ice Crystal is traded for the Magic Lantern.
  - Room 63 ((Hermit Flat, Gwonam)): the Crystal of Vision is traded for the Lantern of Vision.
  - Room 43 ((Shipwreck Cliff, Kulvan)): the Fire Diamond is traded for the level 3 sword.
- Quest items lying in rooms ((`pick_up_perm_tool`)): the Book of Koridai in room 56, the Grapple Berries and Fire Diamond in room 32, the Ice Crystal in room 34, the Winged Helmet in room 40, the Bell in room 7, the Necklace in room 19, the Crystal of Reflection in room 49 and the Crystal of Vision in room 61. Several of them make the room's exit appear straight away.
- The Water of Life spots ((item spots 40 to 44)) are never used up. Every touch heals Link fully.
- Keys are item spots 0 to 10 ((experience bits $80 to $8A)). Key $85 opens two doors, in rooms 28 and 29 ((`check_for_key`)).
- Boss rooms play their boss's cutscene the first time ((`start_boss_first_anim`)): room 7 Goronu, 28 Harlequin, 40 Militron, 54 Glutko, 60 Harlequin again, 61 Lupay, 66 Militron again and 69 Ganon.
- Dark rooms: 6, 19, 32, 34, 35, 36, 39, 45, 58, 59, 60, 61, 66, 67, 68 and 69 ((`set_dark_room`)).
- In room 61, a projectile blocked by the shield bounces back and can hurt any enemy ((`check_collisions`)).

## Characters and enemies

Every sprite in a room is an "enemy" to the code, the people Link talks to included. Each kind has a number ((`enemy_id`)) that picks its graphics, its weapon bits ((`vulnerability`)) and its moves. The rooms are where each one shows up ((from the room enemy lists)).

People:

- 1: Hamish ((room 11))
- 2: Aypo ((room 13))
- 3: Anutu ((room 1))
- 4: Fish Lady ((room 3))
- 5: Morshu ((room 4))
- 6: Gwonam ((rooms 6, 25, 32, 37, 53, 57, 63))
- 7: Alora ((room 22))
- 8: Horgam ((room 24))
- 9: Fairy 1 ((room 17))
- 10: Fairy 2 ((room 17))
- 11: Zorga ((room 20))
- 12: Odranoel ((room 30))
- 13: Suprena ((room 36))
- 14: Clora ((room 42))
- 15: Kulvan ((room 43))
- 16: Zelda ((room 49))
- 17: Droolik ((room 52))
- 18: Zelda ((room 70))

Bosses:

- 21: Goronu ((rooms 7, 49))
- 22: Harlequin ((rooms 28, 60))
- 23: Militron ((rooms 40, 66))
- 24: Glutko ((rooms 54, 56))
- 25: Lupay ((room 61))
- 26: Ganon ((room 69))

Enemies:

- 29: no graphics ((empty)) ((rooms 0, 43, 70))
- 30: Moblin ((rooms 8, 9, 10))
- 31: Moblin ((crouching)) ((rooms 9, 10, 37, 41, 51))
- 32: Moblin ((throwing)) ((rooms 22, 62))
- 33: Daira ((rooms 5, 23))
- 34: Daira ((crouching)) ((rooms 5, 21, 24))
- 35: Daira ((throwing)) ((rooms 0, 21, 24))
- 36: Fire Daira ((room 2))
- 37: Fire Daira ((crouching)) ((rooms 2, 31, 32))
- 39: Lowder ((room 67))
- 40: Octorok ((rooms 31, 46, 48))
- 41: Stalfos ((rooms 6, 7, 58))
- 42: Darknut ((rooms 39, 44))
- 43: Gleeok ((room 19))
- 44: Gleeok head ((room 19))
- 45: Dodongo ((room 51))
- 46: Armos ((rooms 38, 40, 46, 47, 48))
- 47: Jawbee ((black)) ((rooms 27, 53, 54))
- 48: Jawbee ((red)) ((rooms 27, 52, 53, 54))
- 49: Arpagos ((rooms 5, 21, 23, 49, 57))
- 50: Sancrumi ((room 64))
- 51: Abominom ((crouching)) ((rooms 12, 33, 34, 36))
- 52: Abominom ((room 12))
- 53: Boar ((room 25))
- 54: Ghini ((yellow)) ((room 6))
- 55: Ghini ((green)) ((rooms 7, 50, 58))
- 56: Wallmaster ((rooms 28, 45, 47))
- 57: Bot ((room 59))
- 58: Zol ((room 26))
- 59: Phyrandaii ((room 32))
- 60: Goriya ((rooms 14, 26, 29))
- 61: Goriya ((crouching)) ((rooms 8, 11, 14, 15, 16))
- 62: Gohma ((rooms 0, 44))
- 63: Gohma ((room 1))
- 65: Keese ((rooms 15, 18, 26, 28))
- 67: Deeler ((room 27))
- 68: Zora ((rooms 62, 64))
- 69: Bridge ((rooms 14, 15, 31, 51, 64))
- 70: Peahat ((rooms 39, 68))
- 71: Goriya ((no boomerang)) ((room 8))
- 72: Spearfish ((rooms 16, 18, 55))
- 73: Gargoyle ((room 65))
- 74: Arpagos ((rooms 22, 23, 25, 37, 38, 41, 42, 43, 44, 45, 46, 47, 48, 55, 62))
- 75: Boon ((rooms 18, 57, 60))
- 76: Deadite ((rooms 12, 33, 34, 35, 36, 59))
- 77: Sidestepper ((rooms 58, 60))
- 78: Acheman ((rooms 40, 66))
- 80: Spider ((rooms 38, 39))
- 81: Ant ((room 65))
- 82: Rock ((room 65))
- 84: Flying Snake ((room 67))
- 85: Worm ((rooms 66, 68))
- 87: Fire Stalfos ((rooms 45, 50))
- 89: Moa ((room 68))

## Places

The 71 rooms belong to 15 places, one for each spot on the world map. A map spot takes Link into its place ((`area_entries`, `main`)), and the doors lead on from there.

- Crater Cove: rooms 0 to 3 ((map spot 12, enters room 0))
- Goronu: rooms 4 to 7 ((map spot 9, enters room 5))
- Nortinka: rooms 8 to 13 ((map spot 0, enters room 8))
- Spearfish: rooms 14 to 20 ((map spot 14, enters room 14))
- Toyku Lighthouse: rooms 21 to 24 ((map spot 3, enters room 21))
- Harlequin Bazaar: rooms 25 to 30 ((map spot 10, enters room 25))
- Firestone Lake: rooms 31 to 32 ((map spot 6, enters room 31))
- Serigon Caves: rooms 33 to 36 ((map spot 5, enters room 33))
- Militron: rooms 37 to 40 ((map spot 13, enters room 37))
- Shipwreck Cliff: rooms 41 to 43 ((map spot 11, enters room 41))
- Fortress Centrum: rooms 44 to 49 ((map spot 4, enters room 44))
- Glutko: rooms 50 to 56 ((map spot 2, enters room 50))
- Lupay: rooms 57 to 61 ((map spot 1, enters room 57))
- Hermit Flat: rooms 62 to 63 ((map spot 8, enters room 62))
- Ganon's Lair: rooms 64 to 70 ((map spot 7, enters room 64))

## Cutscenes

The cutscenes are numbered by their record in `lanim.rtr` ((`game_won` holds the one to play next, `play_animation` plays it)). Some come in a whole and a short version: the whole one plays the first time and the short one on later visits. Records 51, 59 and 60 have no sound and aren't listed.

- 1: Alora's Missing Necklace ((short))
- 2: Hamish Freezing
- 3: Hamish's Water of Life
- 4: Aypo's Offer
- 5: Aypo Reads the Book
- 6: Anutu's Power Sword
- 7: Anutu Cheers Link On
- 8: Fish Lady's Secret
- 9: Morshu's Store
- 10: Morshu Refuses Sale
- 11: Gwonam on Goronu
- 12: Goronu Taunts Link
- 13: Goronu Dies
- 14: Zorga's Magic ((short))
- 15: Alora's Missing Necklace ((whole))
- 16: Alora's Canteen
- 17: Horgam's Lantern
- 18: Horgam's Advice
- 19: Fairies Life Heart
- 20: Fairies Water of Life
- 21: Zorga's Magic ((whole))
- 22: Zorga's Power Glove
- 23: Gwonam on Harlequin
- 24: Ganon's Offer
- 25: Harlequin Taunts Link
- 26: Harlequin Dies
- 27: Odranoel's Experiment
- 28: Odranoel's Lantern
- 29: Gwonam on Zelda's Capture
- 30: Suprena's Challenge
- 31: Suprena and the Shield
- 32: Gwonam on Militron
- 33: Militron Taunts Link
- 34: Militron's Defeat
- 35: Clora ((whole))
- 36: Clora ((short))
- 37: Kulvan In Chains
- 38: Kulvan Tempers The Sword
- 39: Ganon's Defeat
- 40: Goronu's Revenge
- 41: Gwonam on Glutko
- 42: Droolik's Advice
- 43: Glutko Attacks
- 44: Glutko Dies
- 45: Gwonam on Lupay
- 46: Harlequin's Revenge
- 47: Lupay Taunts Link
- 48: Lupay Dies
- 49: Gwonam's Lantern
- 50: Militron's Revenge
- 52: Opening Credits
- 53: Tutorial
- 54: Opening 1
- 55: Opening 2
- 56: Opening 3
- 57: Ending 1
- 58: Ending 2
- 61: Credits 1
- 62: Credits 2
- 63: Credits 3
- 64: Credits 4
- 65: Credits 5
- 66: Credits 6
- 67: Credits 7
- 68: Credits 8
- 69: Credits 9

## Other

- Random numbers: `seed = (seed * 415821 + 1) mod 65536`, and the result is `seed >> 2`. The seed starts from the time of day ((`randomize`, `init_random`)).
- The screen dims after 13500 frames without input in a room, or 54000 elsewhere ((`screen_dimmer`)). After 1400 frames without input on the title menu, the demo starts ((`mmenu_loop`)).
- Saves: there are 3 slots in NVRAM. Each save needs $62 bytes of game state plus $80 bytes for the file itself ((`verify_save_space`, `save_game_to_NVR`)).
- Any button skips a cutscene ((`play_animation`)).
