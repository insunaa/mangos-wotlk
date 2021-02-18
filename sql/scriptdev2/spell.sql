DROP TABLE IF EXISTS spell_scripts;
CREATE TABLE spell_scripts(
Id INT NOT NULL COMMENT 'Spell ID',
ScriptName CHAR(64) NOT NULL COMMENT 'Core Spell Script Name',
PRIMARY KEY(Id)
);

-- Classic
INSERT INTO spell_scripts(Id, ScriptName) VALUES
(720,'spell_entangle_fankriss'),
(731,'spell_entangle_fankriss'),
(785,'spell_true_fulfillment'),
(794,'spell_initialize_images'),
(802,'spell_mutate_bug'),
(1121,'spell_entangle_fankriss'),
(2400,'spell_anubisath_share_powers'),
(3730,'spell_initialize_image'),
(7054,'spell_forsaken_skill'),
(12639,'spell_summon_hakkar'),
(12948,'spell_hakkar_summoned'),
(13278,'spell_gdr_channel'),
(13493,'spell_gdr_periodic'),
(11610,'spell_gammerita_turtle_camera'),
(16380,'spell_greater_invisibility_mob'),
(17016,'spell_placing_beacon_torch'),
(17244,'spell_anastari_possess'),
(19832,'spell_possess_razorgore'),
(19872,'spell_calm_dragonkin'),
(19873,'spell_destroy_egg'),
(20038,'spell_explosion_razorgore'),
(21651,'spell_opening_capping'),
(23226,'spell_ritual_candle_aura'),
(24693,'spell_hakkar_power_down'),
(25684,'spell_summon_mana_fiends_moam'),
(25813,'spell_conjure_dream_rift'),
(25885,'spell_viscidus_summon_globs'),
(25926,'spell_viscidus_frost_weakness'),
(25937,'spell_viscidus_freeze'),
(26003,'spell_viscidus_suicide'),
(26009,'spell_cthun_periodic_rotate'),
(26084,'spell_aq_whirlwind'),
(26136,'spell_cthun_periodic_rotate'),
(26137,'spell_cthun_rotate_trigger'),
(26152,'spell_cthun_periodic_eye_trigger'),
(26180,'spell_huhuran_wyvern_string'),
(26205,'spell_hook_tentacle_trigger'),
(26217,'spell_cthun_tentacles_summon'),
(26230,'spell_cthun_exit_stomach'),
(26237,'spell_cthun_tentacles_summon'),
(26256,'spell_cthun_check_reset'),
(26275,'spell_wondervolt_trap'),
(26332,'spell_cthun_mouth_tentacle'),
(26398,'spell_cthun_tentacles_summon'),
(26476,'spell_cthun_digestive_acid_periodic'),
(26584,'spell_summon_toxic_slime'),
(26585,'spell_viscidus_despawn_adds'),
(26591,'spell_teleport_image'),
(26608,'spell_viscidus_despawn_adds'),
(26619,'spell_periodic_scarab_trigger'),
(26686,'spell_aq_whirlwind'),
(26767,'spell_cthun_tentacles_summon'),
(26769,'spell_cthun_periodic_eye_trigger'),
(28282,'spell_ashbringer_item'),
(28441,'spell_ab_effect_000');

-- TBC
INSERT INTO spell_scripts(Id, ScriptName) VALUES
(38048,'spell_curse_of_pain'),
(430,'spell_drink'),
(431,'spell_drink'),
(432,'spell_drink'),
(1133,'spell_drink'),
(1135,'spell_drink'),
(1137,'spell_drink'),
(10250,'spell_drink'),
(22734,'spell_drink'),
(27089,'spell_drink'),
(34291,'spell_drink'),
(43706,'spell_drink'),
(46755,'spell_drink'),
(49472,'spell_drink'),
(42144,'spell_horseman_create_water_bucket'),
(42074,'spell_horseman_fire'),
(43885,'spell_horseman_maniacal_laugh'),
(43884,'spell_horseman_maniacal_laugh_delayed'),
(43886,'spell_horseman_maniacal_laugh_delayed'),
(42140,'spell_horseman_start_fire_periodic'),
(42143,'spell_horseman_start_fire_target_test'),
(43711,'spell_horseman_start_fire_target_test'),
(42132,'spell_horseman_start_fire'),
(42079,'spell_horseman_start_fire'),
(43715,'spell_horseman_start_fire'),
(42637,'spell_horseman_conflagrate_periodic'),
(42638,'spell_horseman_conflagrate_target_test'),
(42339,'spell_bucket_lands'),
(42348,'spell_fire_extinguish'),
(42151,'spell_all_fires_out_test'),
(43321,'spell_human_child_laughter'),
(43324,'spell_orc_child_laughter'),
(43323,'spell_human_child_crying'),
(42178,'spell_invisible_campfire_remove'),
(32811,'spell_greater_invisibility_mob'),
(29969,'spell_summon_blizzard'),
(29970,'spell_dispel_blizzard'),
(29979,'spell_massive_magnetic_pull'),
(30004,'spell_flame_wreath'),
(29770,'spell_midnight_mount'),
(29883,'spell_blink_arcane_anomaly'),
(29907,'spell_astral_bite'),
(29690,'spell_inebriate_removal'),
(29448,'spell_moroes_vanish'),
(30115,'spell_sacrifice'),
(31702,'spell_someone_grab_me'),
(31703,'spell_magnetic_pull'),
(31704,'spell_levitate'),
(30166,'spell_shadow_grasp_magtheridon'),
(30410,'spell_shadow_grasp_cube'),
(30658,'spell_quake_magtheridon'),
(30571,'spell_quake_magtheridon_knockback'),
(30425,'spell_portal_attunement'),
(30469,'spell_nether_beam'),
(38546,'spell_face_random_target'),
(30659,'spell_fel_infusion'),
(31427,'spell_allergies'),
(35139,'spell_throw_booms_doom'),
(35150,'spell_nether_charge_passive'),
(37670,'spell_nether_charge_timer'),
(33676,'spell_incite_chaos'),
(30843,'spell_enfeeble'),
(41624,'spell_enfeeble_removal'),
(32264,'spell_shirrak_inhibit_magic'),
(33332,'spell_suppression_blast'),
(32830,'spell_auchenai_possess'),
(32756,'spell_shadowy_disguise'),
(36557,'spell_cursed_scarab_periodic'),
(36560,'spell_cursed_scarab_despawn_periodic'),
(34874,'spell_despawn_underbog_mushrooms'),
(36374,'spell_summon_smoke_beacon'),
(37591,'spell_inebriate_removal'),
(35754,'spell_soccothrates_charge'),
(31617,'spell_raise_dead_hyjal'),
(31624,'spell_raise_dead_hyjal'),
(31625,'spell_raise_dead_hyjal'),
(38028,'spell_watery_grave'),
(34231,'spell_increased_holy_light_healing'),
(33793,'spell_vazruden_liquid_fire_script'),
(33794,'spell_vazruden_liquid_fire_script'),
(30926,'spell_vazruden_liquid_fire_script'),
(36921,'spell_vazruden_liquid_fire_script'),
(30700,'spell_face_highest_threat'),
(36153,'spell_soulbind'),
(35181,'spell_dive_bomb'),
(36460,'spell_ultra_deconsolodation_zapper'),
(37431,'spell_lurker_spout_initial'),
(37429,'spell_lurker_spout_turning'),
(37430,'spell_lurker_spout_turning'),
(37851,'spell_tag_for_single_use'),
(37896,'spell_to_infinity_and_above'),
(38606,'spell_exorcism_feather'),
(38640,'spell_koi_koi_death'),
(38915,'spell_mental_interference'),
(38544,'spell_coax_marmot'),
(40309,'spell_possess_demon_shartuul'),
(40222,'spell_smash_shield'),
(40503,'spell_possession_transfer'),
(41962,'spell_possession_transfer'),
(40821,'spell_touch_of_madness'),
(40824,'spell_madness_rift'),
(40605,'spell_eredar_pre_gate_beam'),
(40563,'spell_throw_axe'),
(40493,'spell_super_jump'),
(41589,'spell_cleansing_flame'),
(41939,'spell_rampaging_charge'),
(40736,'spell_death_blast'),
(41579,'spell_iceblast'),
(40658,'spell_absorb_life'),
(40659,'spell_shartuul_dive_bomb'),
(40557,'spell_flying_attack_aura'),
(40556,'spell_fireball_barrage'),
(40741,'spell_chaos_strike'),
(41629,'spell_chaos_strike_transform'),
(41593,'spell_shivan_shapeshift_form'),
(41594,'spell_shivan_shapeshift_form'),
(41595,'spell_shivan_shapeshift_form'),
(40675,'spell_build_portable_fel_cannon'),
(40777,'spell_stun_rope_attunement'),
(40186,'spell_summon_blossom_move_target'),
(40251,'spell_shadow_of_death'),
(41289,'spell_ethereal_ring_signal_flare'),
(41999,'spell_shadow_of_death_remove'),
(38358,'spell_tidal_surge_caribdis'),
(37676,'spell_insidious_whisper'),
(37750,'spell_clear_consuming_madness'),
(37640,'spell_leotheras_whirlwind_aura'),
(37641,'spell_leotheras_whirlwind_proc'),
(33572,'spell_gronn_lords_grasp'),
(38511,'spell_vashj_persuasion'),
(38573,'spell_spore_drop_effect'),
(33040,'spell_wota_main_cast_and_jump'),
(33049,'spell_wota_main_cast_and_jump'),
(33045,'spell_wota_dot'),
(33048,'spell_wota_search'),
(33229,'spell_wota_remove'),
(36812,'spell_soaring'),
(37910,'spell_soaring'),
(37962,'spell_soaring'),
(37968,'spell_soaring'),
(42783,'spell_wrath_of_the_astromancer'),
(45043,'spell_power_circle'),
(44006,'spell_teleport_self_akilzon'),
(43359,'spell_call_of_the_beast'),
(43681,'spell_inactive'),
(40084,'spell_harpooners_mark'),
(31258,'spell_winterchill_death_and_decay'),
(31436,'spell_split_damage'),
(31447,'spell_mark_of_kazrogal'),
(31347,'spell_azgalor_doom'),
(40214,'spell_dragonmaw_illusion_base'),
(42016,'spell_dragonmaw_illusion_transform'),
(39810,'spell_sparrowhawk_net'),
(34190,'spell_void_reaver_arcane_orb'),
(41034,'spell_spell_absorption'),
(35861,'spell_nether_vapor_summon'),
(35862,'spell_nether_vapor_summon'),
(35863,'spell_nether_vapor_summon'),
(35864,'spell_nether_vapor_summon'),
(35865,'spell_nether_vapor_summon_parent'),
(45960,'spell_nether_vapor_lightning'),
(39497,'spell_remove_weapons'),
(31298,'spell_anetheron_sleep'),
(41910,'spell_alar_phoenix_ember_blast'),
(33985,'spell_use_corpse'),
(34011,'spell_raise_dead'),
(34012,'spell_raise_dead'),
(34019,'spell_raise_dead'),
(41071,'spell_raise_dead'),
(40567,'spell_ogrila_flasks'),
(40568,'spell_ogrila_flasks'),
(40572,'spell_ogrila_flasks'),
(40573,'spell_ogrila_flasks'),
(40575,'spell_ogrila_flasks'),
(40576,'spell_ogrila_flasks'),
(41608,'spell_ogrila_flasks'),
(41609,'spell_ogrila_flasks'),
(41610,'spell_ogrila_flasks'),
(41611,'spell_ogrila_flasks'),
(46837,'spell_ogrila_flasks'),
(46839,'spell_ogrila_flasks'),
(40887,'spell_assist_bt'),
(40892,'spell_fixate_bt'),
(39581,'spell_storm_blink'),
(34630,'spell_scrap_reaver_spell'),
(41476,'spell_veras_vanish'),
(44935,'spell_razorthorn_root'),
(44881,'spell_charm_ravager'),
(44948,'spell_living_flare_detonator'),
(44877,'spell_living_flare_master'),
(44943,'spell_living_flare_unstable'),
(45188,'spell_dawnblade_attack'),
(38858,'spell_queldanas_shoot'),
(34800,'spell_getting_sleepy_aura'),
(43364,'spell_getting_sleepy_aura'),
(37156,'spell_tk_dive'),
(41951,'spell_supremus_random_target'),
(34438,'spell_unstable_affliction'),
(34439,'spell_unstable_affliction'),
(35183,'spell_unstable_affliction'),
(43723,'spell_demon_broiled_surprise'),
(42399,'spell_send_head'),
(43101,'spell_head_requests_body'),
(35831,'spell_ring_of_flame'),
(41914,'spell_parasitic_shadowfiend'),
(41917,'spell_parasitic_shadowfiend'),
(40647,'spell_shadow_prison'),
(33499,'spell_shape_of_the_beast'),
(44329,'spell_fel_crystal_dummy'),
(44320,'spell_mana_rage_selin'),
(40848,'spell_dragonmaw_knockdown_the_aggro_check'),
(40985,'spell_dragonmaw_knockdown_the_aggro_check'),
(40993,'spell_dragonmaw_knockdown_the_aggro_check'),
(41007,'spell_dragonmaw_knockdown_the_aggro_check'),
(41015,'spell_dragonmaw_knockdown_the_aggro_check'),
(41022,'spell_dragonmaw_knockdown_the_aggro_check'),
(45219,'spell_shattered_sun_marksman_shoot'),
(45223,'spell_shattered_sun_marksman_shoot'),
(45229,'spell_shattered_sun_marksman_shoot'),
(45233,'spell_shattered_sun_marksman_shoot'),
(32863,'spell_seed_of_corruption_npc'),
(36123,'spell_seed_of_corruption_npc'),
(38252,'spell_seed_of_corruption_npc'),
(39367,'spell_seed_of_corruption_npc'),
(44141,'spell_seed_of_corruption_npc'),
(45091,'spell_spar_auras'),
(45092,'spell_spar_auras'),
(47108,'spell_clear_energy_feedback'),
(44224,'spell_gravity_lapse_mgt'),
(44232,'spell_clear_flight_mgt'),
(34480,'spell_gravity_lapse_knockup'),
(44226,'spell_gravity_lapse_knockup'),
(49887,'spell_gravity_lapse_knockup'),
(32727,'spell_arena_preparation');

-- Wotlk
INSERT INTO spell_scripts(Id, ScriptName) VALUES
(57073,'spell_drink'),
(61830,'spell_drink'),
(72623,'spell_drink'),
(28728,'spell_feigh_death_pos_aura'),
(41253,'spell_greater_invisibility_mob'),
(42672,'spell_frost_tomb_aura'),
(42796,'spell_ingvar_transform_aura'),
(42912,'spell_summon_banshee'),
(43768,'spell_flying_machine_controls'),
(43770,'spell_grappling_hook'),
(43789,'spell_grappling_beam'),
(47028,'spell_taunka_face_me'),
(47031,'spell_tag_troll'),
(47035,'spell_out_cold'),
(47042,'spell_assemble_cage'),
(47374,'spell_ley_line_focus_ring'),
(47469,'spell_ley_line_focus_ring'),
(47574,'spell_freezing_cloud_aura'),
(47594,'spell_freezing_cloud_aura'),
(47634,'spell_ley_line_focus_ring'),
(47669,'spell_awaken_subboss_aura'),
(47670,'spell_awaken_gortok'),
(47710,'spell_summon_telestra_clones'),
(47711,'spell_telestra_clone_dies_aura'),
(47712,'spell_telestra_clone_dies_aura'),
(47713,'spell_telestra_clone_dies_aura'),
(47747,'spell_charge_rifts'),
(47764,'spell_gravity_well_effect'),
(47941,'spell_crystal_spike_aura'),
(47958,'spell_crystal_spikes'),
(48385,'spell_create_spirit_fount_beam_aura'),
(48590,'spell_avenging_spirits'),
(50442,'spell_crystal_spike_visual_aura'),
(50471,'spell_escape_from_silverbrook_credit_master'),
(50546,'spell_ley_line_focus_item_trigger'),
(50547,'spell_ley_line_focus_item_trigger'),
(50548,'spell_ley_line_focus_item_trigger'),
(47431,'spell_capture_jormungar_spawn'),
(47435,'spell_scrape_corrosive_spit'),
(47447,'spell_corrosive_spit'),
(47938,'spell_twisting_blade'),
(48095,'spell_intense_cold_aura'),
(48268,'spell_container_of_rats'),
(48397,'spell_drop_off_villager'),
(48630,'spell_summon_gauntlet_mobs_periodic_aura'),
(48642,'spell_launch_harpoon'),
(48778,'spell_acherus_deathcharger'),
(49099,'spell_army_of_the_dead'),
(49345,'spell_call_oculus_drake'),
(49346,'spell_ride_oculus_drake_saddle'),
(49460,'spell_ride_oculus_drake_saddle'),
(49461,'spell_call_oculus_drake'),
(49462,'spell_call_oculus_drake'),
(49464,'spell_ride_oculus_drake_saddle'),
(49546,'spell_eagle_eyes'),
(50255,'spell_poisoned_spear'),
(50550,'spell_parachute_aura'),
(50563,'spell_carve_stone_aura'),
(50630,'spell_eject_all_passengers'),
(50742,'spell_ooze_combine'),
(50789,'spell_summon_iron_dwarf_aura'),
(50792,'spell_summon_iron_trogg_aura'),
(50801,'spell_summon_malformed_ooze_aura'),
(50810,'spell_shatter'),
(50824,'spell_summon_earthen_dwarf_aura'),
(50836,'spell_petrifying_grip_aura'),
(51519,'spell_death_knight_initiate_visual'),
(51769,'spell_emblazon_runeblade_aura'),
(51770,'spell_emblazon_runeblade'),
(51774,'spell_taunt_brann'),
(51858,'spell_siphon_of_acherus'),
(51859,'spell_siphon_of_acherus_aura'),
(51904,'spell_summon_ghouls_scarlet_crusade'),
(51973,'spell_siphon_of_acherus_credit'),
(51976,'spell_siphon_of_acherus_credit'),
(51979,'spell_siphon_of_acherus_credit'),
(51981,'spell_siphon_of_acherus_credit'),
(52419,'spell_deflection'),
(52479,'spell_gift_of_the_harvester'),
(52519,'spell_ghoulplosion'),
(52555,'spell_dispel_scarlet_ghoul_credit'),
(52588,'spell_skeletal_gryphon_escape'),
(52694,'spell_recall_eye_of_acherus'),
(52751,'spell_death_gate'),
(52942,'spell_pulsing_shockwave'),
(53035,'spell_summon_anubar_periodic_aura'),
(53036,'spell_summon_anubar_periodic_aura'),
(53037,'spell_summon_anubar_periodic_aura'),
(53110,'spell_devour_humanoid'),
(53177,'spell_web_door_aura'),
(53185,'spell_web_door_aura'),
(54148,'spell_ritual_of_the_sword'),
(54164,'spell_summon_players'),
(54205,'spell_svala_transforming'),
(54259,'spell_splatter_aura'),
(54260,'spell_water_globule_missile'),
(54269,'spell_water_globule_merge'),
(54306,'spell_protective_bubble_aura'),
(54361,'spell_void_shift_aura'),
(55430,'spell_gymers_buddy'),
(55814,'spell_eck_spit'),
(55853,'spell_vortex_aura'),
(55931,'spell_conjure_flame_sphere'),
(56072,'spell_ride_red_dragon_buddy'),
(56105,'spell_vortex'),
(56263,'spell_vortex_aura'),
(56264,'spell_vortex_aura'),
(56265,'spell_vortex_aura'),
(56266,'spell_vortex_aura'),
(56430,'spell_arcane_bomb'),
(57082,'spell_crystal_spikes'),
(57083,'spell_crystal_spikes'),
(57283,'spell_remove_mushroom_power'),
(57473,'spell_arcane_storm'),
(57578,'spell_lava_strike'),
(58418,'spell_portal_to_capital_city'),
(58420,'spell_portal_to_capital_city'),
(59275,'spell_summon_gauntlet_mobs_periodic_aura'),
(59317,'spell_teleporting_dalaran'),
(59331,'spell_poisoned_spear'),
(59743,'spell_void_shift_aura'),
(59820,'spell_drained_aura'),
(59837,'spell_pulsing_shockwave'),
(59858,'spell_summon_malformed_ooze_aura'),
(59859,'spell_summon_iron_trogg_aura'),
(59860,'spell_summon_iron_dwarf_aura'),
(60211,'spell_cauterize'),
(61071,'spell_vortex_aura'),
(61072,'spell_vortex_aura'),
(61073,'spell_vortex_aura'),
(61074,'spell_vortex_aura'),
(61075,'spell_vortex_aura'),
(61187,'spell_twilight_shift_aura'),
(61190,'spell_twilight_shift_aura'),
(61210,'spell_align_disk_aggro'),
(61546,'spell_shatter'),
(62138,'spell_teleport_inside_violet_hold'),
(65869,'spell_disengage'),
(66118,'spell_leeching_swarm_aura'),
(66312,'spell_light_ball_passive'),
(66314,'spell_dark_ball_passive'),
(67009,'spell_nether_power'),
(67322,'spell_burrower_submerge'),
(67470,'spell_pursuing_spikes'),
(67547,'spell_clear_valkyr_essence'),
(67590,'spell_powering_up'),
(67630,'spell_leeching_swarm_aura'),
(68084,'spell_clear_valkyr_touch'),
(68614,'spell_irresistible_cologne_spill_aura'),
(68644,'spell_valentine_boss_validate_area'),
(68645,'spell_rocket_pack'),
(68646,'spell_leeching_swarm_aura'),
(68647,'spell_leeching_swarm_aura'),
(68721,'spell_rocket_pack_periodic'),
(68786,'spell_permafrost_aura'),
(68798,'spell_alluring_perfume_spill_aura'),
(68839,'spell_corrupt_soul_aura'),
(68861,'spell_consume_soul'),
(68870,'spell_soulstorm_visual_aura'),
(68871,'spell_wailing_souls'),
(68875,'spell_wailing_souls_aura'),
(68876,'spell_wailing_souls_aura'),
(68965,'spell_lingering_fumes_targetting'),
(68966,'spell_valentine_trigger_vial_a'),
(68987,'spell_pursuit'),
(69008,'spell_soulstorm_visual_aura'),
(69012,'spell_explosive_barrage_aura'),
(69038,'spell_valentine_trigger_vial_b'),
(69048,'spell_mirrored_soul'),
(69051,'spell_mirrored_soul_proc'),
(69057,'spell_bone_spike_graveyard'),
(69140,'spell_coldflame_targeting'),
(69147,'spell_coldflame'),
(69232,'spell_icy_blast'),
(69347,'spell_necromantic_power'),
(69402,'spell_incinerating_blast'),
(69413,'spell_strangulating_aura'),
(69516,'spell_summon_undead'),
(69828,'spell_halls_of_reflection_clone'),
(70017,'spell_gunship_cannon_fire_aura'),
(70175,'spell_incinerating_blast'),
(70336,'spell_permafrost_aura'),
(70527,'spell_jainas_call'),
(70572,'spell_grip_of_agony_aura'),
(70636,'spell_call_of_sylvanas'),
(70826,'spell_bone_spike_graveyard'),
(70842,'spell_mana_barrier_aura'),
(70895,'spell_dark_transformation'),
(70896,'spell_dark_empowerment'),
(70897,'spell_dark_martyrdom'),
(70900,'spell_dark_transformation_aura'),
(70901,'spell_dark_empowerment_aura'),
(70903,'spell_dark_adherent_martyrdom_aura'),
(71236,'spell_dark_fanatic_martyrdom_aura'),
(71281,'spell_slave_trigger_closest'),
(72088,'spell_bone_spike_graveyard'),
(72089,'spell_bone_spike_graveyard'),
(72180,'spell_aggro_nearest_slave'),
(72202,'spell_blood_link'),
(72254,'spell_mark_fallen_champion'),
(72495,'spell_dark_fanatic_martyrdom_aura'),
(72496,'spell_dark_fanatic_martyrdom_aura'),
(72497,'spell_dark_fanatic_martyrdom_aura'),
(72498,'spell_dark_adherent_martyrdom_aura'),
(72499,'spell_dark_adherent_martyrdom_aura'),
(72500,'spell_dark_adherent_martyrdom_aura'),
(72705,'spell_coldflame_summon'),
(72900,'spell_start_halls_of_reflection_quest'),
(73142,'spell_bone_spike_graveyard_storm'),
(73159,'spell_play_movie'),
(74452,'spell_conflagration_targeting'),
(74455,'spell_conflagration'),
(74502,'spell_enervating_brand_aura'),
(74562,'spell_fiery_combustion_aura'),
(74792,'spell_soul_consumption_aura'),
(74812,'spell_leave_twilight_realm_aura'),
(75396,'spell_clear_debuffs'),
(75415,'spell_rallying_shout');

-- Hunter
INSERT INTO spell_scripts(Id, ScriptName) VALUES
(34026,'spell_kill_command'),
(34477,'spell_misdirection'),
(34501,'spell_expose_weakness'),
(781,'Disengage');

-- Priest
INSERT INTO spell_scripts(Id, ScriptName) VALUES
(10060,'spell_power_infusion'),
(27827,'spell_spirit_of_redemption_heal'),
(33076,'spell_prayer_of_mending'),
(33206,'spell_pain_suppression'),
(34433,'spell_shadowfiend');

-- Paladin
INSERT INTO spell_scripts(Id, ScriptName) VALUES
(21082,'spell_seal_of_the_crusader'),
(20162,'spell_seal_of_the_crusader'),
(20305,'spell_seal_of_the_crusader'),
(20306,'spell_seal_of_the_crusader'),
(20307,'spell_seal_of_the_crusader'),
(20308,'spell_seal_of_the_crusader'),
(27158,'spell_seal_of_the_crusader'),
(53407,'spell_judgement'),
(20271,'spell_judgement'),
(57774,'spell_judgement'),
(53408,'spell_judgement'),
(31789,'spell_righteous_defense');
(40470,'spell_paladin_tier_6_trinket');

-- Warlock
INSERT INTO spell_scripts(Id, ScriptName) VALUES
(126,'spell_eye_of_kilrogg'),
(980,'spell_curse_of_agony'),
(1014,'spell_curse_of_agony'),
(6217,'spell_curse_of_agony'),
(11711,'spell_curse_of_agony'),
(11712,'spell_curse_of_agony'),
(11713,'spell_curse_of_agony'),
(27218,'spell_curse_of_agony'),
(1454,'spell_life_tap'),
(1455,'spell_life_tap'),
(1456,'spell_life_tap'),
(11687,'spell_life_tap'),
(11688,'spell_life_tap'),
(11689,'spell_life_tap'),
(27222,'spell_life_tap'),
(57946,'spell_life_tap'),
(27243,'spell_seed_of_corruption'),
(47835,'spell_seed_of_corruption'),
(47836,'spell_seed_of_corruption'),
(27285,'spell_seed_of_corruption_damage'),
(47833,'spell_seed_of_corruption_damage'),
(47834,'spell_seed_of_corruption_damage'),
(35696,'spell_demonic_knowledge'),
(30108,'spell_unstable_affliction'),
(30404,'spell_unstable_affliction'),
(30405,'spell_unstable_affliction'),
(32379,'spell_shadow_word_death'),
(32996,'spell_shadow_word_death'),
(19505,'spell_devour_magic'),
(19731,'spell_devour_magic'),
(19734,'spell_devour_magic'),
(19736,'spell_devour_magic'),
(27276,'spell_devour_magic'),
(27277,'spell_devour_magic');

-- Pet Scaling
INSERT INTO spell_scripts(Id, ScriptName) VALUES
(34902,'spell_hunter_pet_scaling_1'),
(34903,'spell_hunter_pet_scaling_2'),
(34904,'spell_hunter_pet_scaling_3'),

(34947,'spell_warlock_pet_scaling_1'),
(34956,'spell_warlock_pet_scaling_2'),
(34957,'spell_warlock_pet_scaling_3'),
(34958,'spell_warlock_pet_scaling_4'),

(35657,'spell_mage_pet_scaling_1'),
(35658,'spell_mage_pet_scaling_2'),
(35659,'spell_mage_pet_scaling_3'),
(35660,'spell_mage_pet_scaling_4'),

(35661,'spell_priest_pet_scaling_1'),
(35662,'spell_priest_pet_scaling_2'),
(35663,'spell_priest_pet_scaling_3'),
(35664,'spell_priest_pet_scaling_4'),

(35665,'spell_elemental_pet_scaling_1'),
(35666,'spell_elemental_pet_scaling_2'),
(35667,'spell_elemental_pet_scaling_3'),
(35668,'spell_elemental_pet_scaling_4'),

(35669,'spell_druid_pet_scaling_1'),
(35670,'spell_druid_pet_scaling_2'),
(35671,'spell_druid_pet_scaling_3'),
(35672,'spell_druid_pet_scaling_4'),

(35674,'spell_enhancement_pet_scaling_1'),
(35675,'spell_enhancement_pet_scaling_2'),
(35676,'spell_enhancement_pet_scaling_3'),

(36186,'spell_infernal_pet_scaling_1'),
(36188,'spell_infernal_pet_scaling_2'),
(36189,'spell_infernal_pet_scaling_3'),
(36190,'spell_infernal_pet_scaling_4');

-- Warrior
INSERT INTO spell_scripts(Id, ScriptName) VALUES
(5308,'spell_warrior_execute'),
(20658,'spell_warrior_execute'),
(20660,'spell_warrior_execute'),
(20661,'spell_warrior_execute'),
(20662,'spell_warrior_execute'),
(25234,'spell_warrior_execute'),
(25236,'spell_warrior_execute'),
(47470,'spell_warrior_execute'),
(47471,'spell_warrior_execute'),
(20647,'spell_warrior_execute_damage'),
(34428,'spell_warrior_victory_rush');

-- Mage
INSERT INTO spell_scripts(Id, ScriptName) VALUES
(11213,'spell_arcane_concentration'),
(12574,'spell_arcane_concentration'),
(12575,'spell_arcane_concentration'),
(12576,'spell_arcane_concentration'),
(12577,'spell_arcane_concentration');

-- Druid
INSERT INTO spell_scripts(Id, ScriptName) VALUES
(5215,'spell_stealth'),
(6783,'spell_stealth'),
(9913,'spell_stealth'),
(8936,'spell_regrowth'),
(8938,'spell_regrowth'),
(8939,'spell_regrowth'),
(8940,'spell_regrowth'),
(8941,'spell_regrowth'),
(9750,'spell_regrowth'),
(9856,'spell_regrowth'),
(9857,'spell_regrowth'),
(9858,'spell_regrowth'),
(26980,'spell_regrowth'),
(48442,'spell_regrowth'),
(48443,'spell_regrowth');

-- Rogue
INSERT INTO spell_scripts(Id, ScriptName) VALUES
(1784,'spell_stealth'),
(1785,'spell_stealth'),
(1786,'spell_stealth'),
(1787,'spell_stealth'),
(1856,'spell_vanish'),
(1857,'spell_vanish'),
(26889,'spell_vanish'),
(14185,'spell_preparation');

-- Shaman
INSERT INTO spell_scripts(Id, ScriptName) VALUES
(974,'spell_earth_shield'),
(32593,'spell_earth_shield'),
(32594,'spell_earth_shield'),
(49283,'spell_earth_shield'),
(49284,'spell_earth_shield'),
(70811,'spell_item_shaman_t10_elemental_2p_bonus'),
(324,'spell_damage_trigger_shield'),
(325,'spell_damage_trigger_shield'),
(905,'spell_damage_trigger_shield'),
(945,'spell_damage_trigger_shield'),
(8134,'spell_damage_trigger_shield'),
(8788,'spell_damage_trigger_shield'),
(10431,'spell_damage_trigger_shield'),
(10432,'spell_damage_trigger_shield'),
(24398,'spell_damage_trigger_shield'),
(25469,'spell_damage_trigger_shield'),
(25472,'spell_damage_trigger_shield'),
(33736,'spell_damage_trigger_shield'),
(49280,'spell_damage_trigger_shield'),
(49281,'spell_damage_trigger_shield'),
(52127,'spell_damage_trigger_shield'),
(52129,'spell_damage_trigger_shield'),
(52131,'spell_damage_trigger_shield'),
(52134,'spell_damage_trigger_shield'),
(52136,'spell_damage_trigger_shield'),
(52138,'spell_damage_trigger_shield'),
(57960,'spell_damage_trigger_shield');

-- Battleground and Outdoor PvP
INSERT INTO spell_scripts(Id, ScriptName) VALUES
-- Arathi basin
(23936,'spell_battleground_banner_trigger'),
(23932,'spell_battleground_banner_trigger'),
(23938,'spell_battleground_banner_trigger'),
(23935,'spell_battleground_banner_trigger'),
(23937,'spell_battleground_banner_trigger'),
-- Alterac Valley
(24677,'spell_battleground_banner_trigger'),
-- Zangarmarsh
(32433,'spell_outdoor_pvp_banner_trigger'),
(32438,'spell_outdoor_pvp_banner_trigger'),
-- Strand of the Ancients
(52365,'spell_split_teleport_boat'),
(52459,'spell_end_of_round'),
(52528,'spell_split_teleport_boat'),
(53464,'spell_split_teleport_boat'),
(53465,'spell_split_teleport_boat'),
(54640,'spell_teleport_sota'),
-- Wintergrasp
(56659,'spell_build_wintergrasp_vehicle'),
(56662,'spell_build_wintergrasp_vehicle'),
(56664,'spell_build_wintergrasp_vehicle'),
(56666,'spell_build_wintergrasp_vehicle'),
(56668,'spell_build_wintergrasp_vehicle'),
(56670,'spell_build_wintergrasp_vehicle'),
(58622,'spell_teleport_lake_wintergrasp'),
(61409,'spell_build_wintergrasp_vehicle'),
-- Isle of Conquest
(35092,'spell_battleground_banner_trigger'),
(65826,'spell_battleground_banner_trigger'),
(65825,'spell_battleground_banner_trigger'),
(66630,'spell_gunship_portal_click'),
(66637,'spell_gunship_portal_click'),
(66686,'spell_battleground_banner_trigger'),
(66687,'spell_battleground_banner_trigger'),
(68077,'spell_repair_cannon_aura'),
(68078,'spell_repair_cannon');
