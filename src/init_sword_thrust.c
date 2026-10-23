/* init_sword_thrust: starts a sword thrust (damage 1) unless Link is already thrusting, on a
   ladder or using a rope. In the air or on a slope he uses the no-lunge thrust (hero action 7),
   crouching action 10, otherwise action 6 which lunges forward. Returns 1 if started. */

#include "game.h"

long init_sword_thrust(void)
{
    if (link_thrusting || climbing_blk48 || climbing_blk25 || rope_out)
        return 0;
    link_idle_timer = 0;
    attack_damage[0] = 1;    /* sword damage */
    link_anim_step = 0;
    link_thrusting = 1;
    thrust_lunge = 1;
    thrust_lunged = 0;
    if (link_riding)
        thrust_lunge = 0;
    get_sprite_block(link_x, link_y, &jump_info);
    if (jump_rising || jump_falling || jump_info_ground || jump_info.left || jump_info.right) {
        go_find_hero_action(7);    /* thrust without lunge */
        thrust_lunge = 0;
    } else if (link_crouching)
        go_find_hero_action(10);    /* crouching thrust */
    else
        go_find_hero_action(6);    /* standing thrust (lunges 4 pixels) */
    anim_frame_idx = 0;
    return 1;
}
