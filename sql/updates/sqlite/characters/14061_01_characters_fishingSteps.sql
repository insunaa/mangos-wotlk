ALTER TABLE character_db_version RENAME COLUMN required_14044_01_characters_extend_state TO required_14061_01_characters_fishingSteps;

ALTER TABLE characters ADD COLUMN `fishingSteps` NOT NULL DEFAULT '0';


