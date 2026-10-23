/* big_jump_up: the rising part of a big jump (Winged Helmet): moves Link up jump_heights[step]
   cells per frame (3, 3, 2, 2) until he hits a ceiling, steering him left or right; then he falls.
   sword_thrust and check_map_while_jump_descent below also use jump_heights. */

#include "game.h"

long jump_heights[5];

void big_jump_up(void)
{
    long i;

    if (jump_step < 4) {
        for (i = 1; i <= jump_heights[jump_step]; ++i) {
            get_sprite_block(link_x, link_y - 1, &jump_info);
            if (jump_info_solid) {
                jump_rising = 0;
                jump_falling = 1;
                anim_frame_idx = 1;
                break;
            }
            --link_y;
            --link_screen_y;
            --link_sprite_y;
            link_moved = 1;
        }
    }
    if (link_dir == 2)
        jump_up_dir(1);
    else
        jump_up_dir(-1);
    if (jump_rising == 1) {
        ++jump_step;
        if (jump_step == 4) {
            jump_rising = 0;
            jump_falling = 1;
            jump_step = 9;
            anim_frame_idx = 1;
        }
    } else
        jump_step = 9;
    link_anim_step = 0;
    link_idle_timer = 0;
}

/* sword_thrust: one frame of a sword thrust: keeps a mid-air thrust rising/falling, does the
   4-pixel lunge on frame 0 (taken back on frame 1), slides on slopes, ends at the last frame. */
void sword_thrust(void)
{
    long i;

    get_sprite_block(link_x, link_y, &jump_info);
    if (jump_info.left || jump_info.right || jump_info_ground)
        thrust_lunge = 0;
    if (jump_rising) {
        for (i = 1; i <= jump_heights[jump_step]; ++i) {
            get_sprite_block(link_x, link_y - 1, &jump_info);
            if (jump_info_solid) {
                jump_rising = 0;
                jump_falling = 1;
                break;
            }
            --link_y;
            --link_screen_y;
            --link_sprite_y;
        }
        if (jump_rising) {
            ++jump_step;
            if (jump_step == 4) {
                jump_rising = 0;
                jump_falling = 1;
                --jump_step;
            }
        }
    } else if (jump_falling) {
        check_map_while_jump_descent(0);
        if (jump_falling) {
            jump_step -= 1;
            if (jump_step == -1)
                jump_falling = 0;
        }
        if (!jump_falling) {
            get_sprite_block(link_x, link_y, &jump_info);
            if (!jump_info.right && !jump_info.left) {
                go_find_hero_action(6);
                cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
                thrust_lunged = 0;
            }
        }
    } else if (!anim_frame_idx) {
        thrust_lunged = 0;
        if (thrust_lunge) {
            if (link_dir == 2) {
                if (link_sprite_x + 18 < 94) {
                    link_sprite_x += 4;
                    thrust_lunged = 1;
                }
            } else if (link_sprite_x > 5) {
                link_sprite_x -= 4;
                thrust_lunged = 1;
            }
        }
    } else if (anim_frame_idx == 1 && thrust_lunged) {
        if (link_dir == 2)
            link_sprite_x -= 4;
        else
            link_sprite_x += 4;
    }
    if (!jump_rising && !jump_falling && !link_riding) {
        get_sprite_block(link_x, link_y, &jump_info);
        if (jump_info.right)
            slide(1, (long)&jump_info.right);
        else if (jump_info.left)
            slide(-1, (long)&jump_info.left);
        else
            for (i = 0; i < 3; ++i) {
                get_sprite_block(link_x, link_y, &jump_info);
                if (jump_info.right) {
                    slide(1, (long)&jump_info.right);
                    break;
                }
                if (jump_info.left) {
                    slide(-1, (long)&jump_info.left);
                    break;
                }
                if (jump_info_ground || jump_info.door) {
                    ++link_y;
                    ++link_screen_y;
                    ++link_sprite_y;
                }
            }
    }
    ++anim_frame_idx;
    if (hero_actions[hero_action].frames[anim_frame_idx] == 999)
        end_thrust();
}

/* check_map_while_jump_descent: moves Link down (and dx sideways) for one falling frame of a
   jump, landing on floor, track, steps, slopes or ladder tops; a deadly block stops him. */
void check_map_while_jump_descent(long dx)
{
    long i;

    get_sprite_block(link_x + dx, link_y - 1, &jump_info);
    if (jump_info.deadly) {
        link_moved = 0;
        return;
    }
    if (dx > 0 && jump_info.left || dx < 0 && jump_info.right) {
        --link_y;
        --link_screen_y;
        --link_sprite_y;
        jump_falling = 0;
        set_link_position(dx);
        link_anim_step = 0;
    } else if (!jump_info_solid) {
        if (jump_info.step) {
            do {
                --link_y;
                --link_screen_y;
                --link_sprite_y;
                get_sprite_block(link_x + dx, link_y - 1, &jump_info);
            } while (jump_info.step == 1);
            jump_falling = 0;
        } else if (jump_info.f12 || jump_info.f13 || jump_info.f14 || jump_info.f15 || jump_info.track) {
            --link_y;
            --link_screen_y;
            --link_sprite_y;
            jump_falling = 0;
        } else {
            get_sprite_block(link_x + dx, link_y, &jump_info);
            if (jump_info.deadly) {
                link_moved = 0;
                return;
            }
            if (dx > 0 && jump_info.left || dx < 0 && jump_info.right) {
                jump_falling = 0;
                set_link_position(dx);
                link_anim_step = 0;
            } else if (!jump_info_solid) {
                if (jump_info.floor || jump_info.track)
                    jump_falling = 0;
                else if (jump_info.climb_a && (old_get_sprite_block(link_x + dx, link_y + 1) != 48
                                          || old_get_sprite_block(link_x + dx, link_y - 1) != 48))
                    jump_falling = 0;
                else if (jump_info.climb_b && (old_get_sprite_block(link_x + dx, link_y + 1) != 25
                                          || old_get_sprite_block(link_x + dx, link_y - 1) != 25))
                    jump_falling = 0;
                else if (jump_step < 4)
                    for (i = 1; i <= jump_heights[jump_step]; ++i) {
                        get_sprite_block(link_x + dx, link_y + 1, &jump_info);
                        if (jump_info.deadly) {
                            link_moved = 0;
                            return;
                        }
                        if (dx >= 0 && jump_info.left || dx <= 0 && jump_info.right) {
                            ++link_y;
                            ++link_screen_y;
                            ++link_sprite_y;
                            jump_falling = 0;
                            set_link_position(dx);
                            link_anim_step = 0;
                            break;
                        }
                        if (jump_info_solid) {
                            jump_falling = 0;
                            break;
                        }
                        ++link_y;
                        ++link_screen_y;
                        ++link_sprite_y;
                        if (jump_info.floor || jump_info.track) {
                            jump_falling = 0;
                            break;
                        }
                        if (jump_info.climb_a) {
                            if (old_get_sprite_block(link_x + dx, link_y + 1) != 48
                                    || old_get_sprite_block(link_x + dx, link_y - 1) != 48) {
                                jump_falling = 0;
                                break;
                            }
                        } else if (jump_info.climb_b) {
                            if (old_get_sprite_block(link_x + dx, link_y + 1) != 25
                                    || old_get_sprite_block(link_x + dx, link_y - 1) != 25) {
                                jump_falling = 0;
                                break;
                            }
                        }
                    }
            }
        }
    }
    if (jump_info_solid)
        return;
    set_link_position(dx);
    if (jump_step < 1 && jump_falling && !jump_info.track) {
        get_sprite_block(link_x + dx, link_y - 1, &jump_info);
        if (jump_info_ground) {
            get_sprite_block(link_x + dx, link_y, &jump_info);
            if (!jump_info_solid && !jump_info.door && !jump_info.climb_a && !jump_info.climb_b)
                set_link_position(dx);
        }
    }
}
