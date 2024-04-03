ALTER TABLE db_version RENAME COLUMN required_14081_01_mangos_precision_decimal TO required_14082_01_mangos_spell_template;

ALTER TABLE `spell_template`
	ADD COLUMN `EffectBonusCoefficientFromAP1` NOT NULL DEFAULT '0',
	ADD COLUMN `EffectBonusCoefficientFromAP2` NOT NULL DEFAULT '0',
	ADD COLUMN `EffectBonusCoefficientFromAP3` NOT NULL DEFAULT '0';

DROP TABLE `spell_bonus_data`;
