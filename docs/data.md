# The game's data

The files `cdi_link` reads from the disc and the tables in its globals. Only the layouts are written down here, the actual values come from your own disc: `python3 tools/a5init.py` lists the globals' starting values in `build/a5init.txt`, which is never published.

Units used on this page:

- A **cell** is 4 pixels. Positions in the game are mostly in cells ((sprites are drawn at `x * 4`)).
- A **sector** is a 2048-byte CD-i sector. 75 sectors make one second of disc.

## Files on the disc

The program opens three real-time files ((RTR)) and plays records from them with `ss_play`. A CD-i "play" streams sectors into memory buffers, as described by a PCB and PCL lists ((see below)). Each load seeks to a sector with `lseek`, starts `ss_play`, and waits until the signal handler sets `play_done`. If it sets `play_failed` instead, `play_error` tries again, up to three times, with another copy of the file.

`ldata.rtr`, opened as `data_path` ((`ldata1.rtr` and `ldata2.rtr` are copies used after read errors)):

- Sector 0: the world map ((`load_world`)): the palette file, the map image and the label images.
- Sector $AE: menu icons, Link's graphics and palettes ((`initial_load`)).
- Room `r` at sector `r * $C0 + $C9` ((`load_playfield`)): the enemy list and enemy records, enemy graphics and collision maps, the palette, the block map and the room image. A full load also brings the projectile and item graphics.
- Sector $3956: the sound effects ((`load_effects`, 6 channels)).
- The other versions use other sector numbers; see `include/version.h`.

`lanim.rtr`, opened as `anim_path`:

- Animations and backgrounds, in records of 20 seconds. Record `n` starts at sector `(n - 1) * 1500`. Used by `play_animation`, `play_background_animation`, `load_backgrounds` and `load_piracy` ((record $3D, or $41 on a PAL machine)).

`lmusic.rtr`, opened as `music_path`:

- Music, in 2-minute blocks of 8 tracks played side by side. Track `t` is in block `t / 8`, channel `t % 8` ((`start_music`)).

`/nvr/link<n>.nvri`, opened by `nvr_open` and `nvr_create`:

- Saved games, in the player's NVRAM ((see below)).

The intro logo is a seperate program, `cdi_bumper`, with its data module `cdi_bumpdata`. `play_bumper` loads and runs it with `os9exec`.

### Rooms

- `room_widths[72]` and `room_heights[72]`: each room's size in screens. A screen is 384 x 240 pixels, or 96 x 60 cells. A room scrolls either sideways or up and down ((`init_playfield_size`)).
- `block_map`: one byte per cell of the room, giving the kind of block ((solid, floor, slope, ladder, door, and so on)). `get_sprite_block` turns the blocks around a point into a `struct block_info`. Block codes 77, 79, 84, 89, 91 and 94 are the six doors: walking into one leaves the room through exit 0 to 5.
- `area_entries[]` ((10 bytes each, in `main.c`)): where the game enters a room from the world map. Each entry has the room, Link's position ((`entry_x`, `entry_y`)) and the scroll position ((`scroll_x`, `scroll_y`)), in cells.
- `area_exits[]` ((same layout)) and `exit_base[72]`: exit `n` ((door 0 to 5)) of room `r` is `area_exits[exit_base[r] + n]`. It gives the room on the other side and where Link appears there.
- `dark_rooms[16]`: the rooms ((numbered from 1)) that are dark without the lantern ((`set_dark_room`)).
- `tool_spots[54]` ((`struct tool_spot`, 6 bytes each)): items lying in rooms, with their kind, room, x and y. An item that was picked up is remembered as an "experience" bit ((`check_if_have_tool`)).
- `env_projectiles`: for each room, the projectiles fired by the room itself ((14 longs each: the arguments of `init_projectile`)).

### Enemies

A room's enemy list ((`enemy_list`)) gives the kinds of enemies in it ((`enemy_types[]`: an id and a graphics block)). The enemy records follow ((`enemy_data`, 176 bytes each, `struct enemy`)).

Each sprite runs a "macro", which is an animation script ((`macros[]`, `sprite_macros`, `macro_ptrs`)). It is moved by one of the `move_type_<n>` routines, chosen by the record's type ((`init_type_<n>`)).

`vulnerability[]` holds, for each enemy id, the bits of the weapons that hurt it ((`check_vulnerablility`)).

Enemy graphics are compiled sprites: small pieces of 68000 code that draw the picture. `struct enemy_gfx` gives the code offset of each of the 24 frames ((called with the screen position to draw at)) and a colour for each frame.

Collision maps ((`cmaps[]`)) hold $3A bytes per frame: 14 rows of hit mask, 14 rows of shield mask, an offset and a width.

### The world map

- `labels[16]` ((`struct label`, 18 bytes each)): the place names shown on the world map. Each has where the label image goes ((x, y, w, h in pixels)), the hotspot that shows it ((in map coordinates)) and a group number. Labels in the same group belong to one place.
- `areas[16]` and `initial_area_tab[16]`: which places are open ((1)). `open_up_area` opens more of them as the game goes on.
- `help_demo`: the script for the world map help ((`get_help_instruction`)). It is a list of bytes: move commands ((a count, then direction bits 1 right, 2 left, 4 down and 8 up, with the buttons in the top bits)), $80 to $82 for voice-over commands, and $FF for the end.

## Tables in the globals

- `hero_actions` and `link_frames_<n>`: Link's actions. Each has an id and a list of frames. A frame list is a list of shorts that ends with 999.
- `walk_frames_left`, `walk_frames_right` and `death_frames`: the frame lists of walking enemies and of a dying sprite.
- `jump_heights[4]`: how many cells Link rises in each step of a jump.
- `proj_frames_<n>` and `proj_defs`: projectile animations ((frame numbers ending with $FF)) and projectile kinds ((`struct proj_def`)).
- `item_names[25]`: two lines of text per item, for the "you got" sign.
- `mm_text`, `mm_titles` and `mm_rows`: the main menu's options, titles and number of rows.
- `low_bits_mask`, `high_bits_mask` and `bit_masks`: bit masks for the collision tests.
- `dcp_regs_a` to `dcp_regs_d` and `color_regs`: display instructions ((FCT)) written at startup and after the logo.
- `cursor_data`: the 16 x 16 pointer image ((`gc_def`)).
- `heart_x` and `heart_w`: where the life hearts are drawn.
- `initial_counts`: item counts at the start of a game.
- `sound_chan`, `sound_len` and `chan_offsets`: the sound effects' channels and lengths ((in units of $900 bytes)).
- `nvr_key` and `nvr_magic`: the alphabet used to pack save names, and "ALL" ((any host)).

## Display ((CD-i video))

The game uses both CD-i video planes. Each is an image with a palette ((CLUT)). Plane A is in front ((menus, sprites, signs)) and plane B is behind ((the map, the playfield, backgrounds)).

The display is controlled by tables of instructions:

- A **field control table** for each plane ((`fct_a`, `fct_b`)): instructions run once per frame, such as loading the palette, setting the display mode, and pointing to the line control table.
- **Line control tables** ((`lct_plane_a1/a2/b1/b2`, `lct_head_a/b`, and `lct_pal_a/b` on 625-line machines)): instructions for each screen line, mostly "this line's pixels start here" ((`dc_wrli`)). Changing which table a head table links to switches between two buffers ((`flip_A`, `flip_backgrounds`)), and rewriting a table scrolls the picture.

Memory comes from each plane's own video RAM ((`srqcmem`, with colour $80 for plane A and $81 for plane B)). `plane_a_mem` holds plane A's two display buffers and a data area. `plane_b_mem` holds plane B's three buffers and the room data ((block map, collision maps, sprite buffers)).

## Loading: PCB and PCL

`ss_play` takes a **PCB** ((play control block, `struct pcb`)). It holds a status, a signal, a record count, channel masks, and the lists of video, audio and data channels.

Each list ((CIL)) holds a **PCL** ((play control list entry, `struct pcl`)) for each channel. A PCL has a control byte, a signal, the next PCL, a buffer address, the buffer size in sectors, an error record and a sector count.

`set_pcb`, `set_pcl` and `reset_pcl` fill them in. The signal handler ((`signal_handler`)) receives the end-of-record and error signals.

## Saved games ((NVRAM))

A save is a file `/nvr/link<n>.nvri` in the player's non-volatile memory ((`save_game_to_NVR`, `load_game_from_NVR`)).

The file starts with an 8-byte header ((`struct file_pos`)): a checksum in the low 14 bits of the first word, then the offsets of the host string, the info string and the data. The two strings are packed 4 bits per character using `nvr_key` ((`nvr_pack_name`)). The data is the game state, gathered into `mem_space` ((`encode_mem_space`, `decode_mem_space`)).

## Starting values of the globals

MPW packs the starting values of the globals into a block of data. A routine in code segment 8 unpacks it at startup, before `main` runs. `tools/a5init.py` decodes it, and its description explains the format.

95 of the values are pointers to other globals, which are fixed up at startup: the menu strings, `item_names`, `mkt_demo_ptr`, `voice_ptr`, `demo_ptr` and a few more.
