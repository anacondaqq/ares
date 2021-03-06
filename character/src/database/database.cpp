#include "database.hpp"

ares::character::database::database(std::shared_ptr<spdlog::logger> log, const config::postgres_config& conf) :
  ares::database<database>(log, conf->dbname, conf->host, conf->port, conf->user, conf->password) {
  SPDLOG_TRACE(log, "ares::character::database::database");

  if (pqxx_conn_) {
    // Prepare statements
    pqxx_conn_->prepare("account_create", R"(
WITH s AS (
 INSERT INTO "account_slots" ("aid", "normal_slots", "premium_slots", "billing_slots", "creatable_slots", "playable_slots") VALUES ($1, $2, $3, $4, $5, $6)) 
INSERT INTO "account_storage" ("aid", "bank_vault", "max_storage") VALUES ($1, $7, $8);

)");
    
    pqxx_conn_->prepare("account_slots_for_aid", R"(
SELECT "normal_slots", "premium_slots", "billing_slots", "creatable_slots", "playable_slots"
FROM "account_slots" WHERE ("aid" = $1) 
)");

    pqxx_conn_->prepare("account_storage_for_aid", R"(
SELECT "bank_vault", "max_storage" FROM "account_storage" WHERE ("aid" = $1)
)");

    pqxx_conn_->prepare("num_chars_for_aid", R"(
SELECT count(*) AS cnt FROM "characters" WHERE ("aid" = $1) AND ("slot" < $2)
)");

    pqxx_conn_->prepare("character_info_for_aid", R"(
SELECT "characters"."id" AS id, "slot", "characters"."name" AS name, "sex", "job", "base_level", "job_level", "base_exp", "job_exp", "zeny",
  "str", "agi", "vit", "int", "dex", "luk",
  "max_hp", "hp", "max_sp", "sp", "job_point", "skill_point", "effect_state", "body_state", "health_state", "virtue", "honor",
  "head", "body", "weapon", "robe", "shield", "head_top", "head_mid", "head_bottom", "head_palette", "body_palette",
  "char_save_location"."map_id" AS "save_map_id", "char_save_location"."x" AS "save_x", "char_save_location"."y" AS "save_y",
  "char_last_location"."map_id" AS "last_map_id", "char_last_location"."x" AS "last_x", "char_last_location"."y" AS "last_y",
  "delete_date", "rename"
FROM "characters"
JOIN "char_appearance" ON ("char_appearance"."cid" = "characters"."id")
JOIN "char_stats" ON ("char_stats"."cid" = "characters"."id")
JOIN "char_save_location" ON ("char_save_location"."cid" = "characters"."id")
JOIN "char_last_location" ON ("char_last_location"."cid" = "characters"."id")
WHERE ("aid" = $1) AND ("slot" < $2)
)");
    
    pqxx_conn_->prepare("character_info", R"(
SELECT "characters"."id" AS id, "slot", "characters"."name" AS name, "sex", "job", "base_level", "job_level", "base_exp", "job_exp", "zeny",
  "str", "agi", "vit", "int", "dex", "luk",
  "max_hp", "hp", "max_sp", "sp", "job_point", "skill_point", "effect_state", "body_state", "health_state", "virtue", "honor",
  "head", "body", "weapon", "robe", "shield", "head_top", "head_mid", "head_bottom", "head_palette", "body_palette",
  "char_save_location"."map_id" AS "save_map_id", "char_save_location"."x" AS "save_x", "char_save_location"."y" AS "save_y",
  "char_last_location"."map_id" AS "last_map_id", "char_last_location"."x" AS "last_x", "char_last_location"."y" AS "last_y",
  "delete_date", "rename"
FROM "characters"
JOIN "char_appearance" ON ("char_appearance"."cid" = "characters"."id")
JOIN "char_stats" ON ("char_stats"."cid" = "characters"."id")
JOIN "char_save_location" ON ("char_save_location"."cid" = "characters"."id")
JOIN "char_last_location" ON ("char_last_location"."cid" = "characters"."id")
WHERE ("characters"."id" = $1)
)");
    
    pqxx_conn_->prepare("character_info_for_slot", R"(
SELECT "characters"."id" AS id, "slot", "characters"."name" AS "name", "sex", "job", "base_level", "job_level", "base_exp", "job_exp", "zeny",
  "str", "agi", "vit", "int", "dex", "luk",
  "max_hp", "hp", "max_sp", "sp", "job_point", "skill_point", "effect_state", "body_state", "health_state", "virtue", "honor",
  "head", "body", "weapon", "robe", "shield", "head_top", "head_mid", "head_bottom", "head_palette", "body_palette",
  "char_save_location"."map_id" AS "save_map_id", "char_save_location"."x" AS "save_x", "char_save_location"."y" AS "save_y",
  "char_last_location"."map_id" AS "last_map_id", "char_last_location"."x" AS "last_x", "char_last_location"."y" AS "last_y",
  "delete_date", "rename"
FROM "characters"
JOIN "char_appearance" ON ("char_appearance"."cid" = "characters"."id")
JOIN "char_stats" ON ("char_stats"."cid" = "characters"."id")
JOIN "char_save_location" ON ("char_save_location"."cid" = "characters"."id")
JOIN "char_last_location" ON ("char_last_location"."cid" = "characters"."id")
WHERE ("aid" = $1) AND ("slot" = $2)
)");

    pqxx_conn_->prepare("make_char_create_cid", R"(
INSERT INTO "characters" ("aid", "slot", "name", "sex", "job", "rename") VALUES ($1, $2, $3, $4, $5, 0)
)");
    
    pqxx_conn_->prepare("cid_by_name", R"(
SELECT "id" FROM "characters" WHERE "name" = $1
)");

    pqxx_conn_->prepare("make_char_create_stats", R"(
INSERT INTO "char_stats" ("cid", "base_level", "job_level", "base_exp", "job_exp", "zeny",
 "str", "agi", "vit", "int", "dex", "luk", "max_hp", "hp", "max_sp", "sp",
 "job_point", "skill_point", "effect_state", "body_state", "health_state", "virtue", "honor")
VALUES ($1, 1, 1, 0, 0, $2, 1, 1, 1, 1, 1, 1, $3, $3, $4, $4, 48, 0, 0, 0, 0, 0, 0)
)");

    pqxx_conn_->prepare("make_char_create_appearance", R"(
INSERT INTO "char_appearance" ("cid", "head", "body", "weapon", "shield", "robe", "head_top", "head_mid", "head_bottom", "head_palette", "body_palette")
VALUES ($1, $2, 0, 0, 0, 0, 0, 0, 0, $3, 0)
)");

    pqxx_conn_->prepare("make_char_create_last_location", R"(
INSERT INTO "char_last_location" ("cid", "map_id", "x", "y")
VALUES ($1, $2, $3, $4)
)");

    pqxx_conn_->prepare("make_char_create_save_location", R"(
INSERT INTO "char_save_location" ("cid", "map_id", "x", "y")
VALUES ($1, $2, $3, $4)
)");

    pqxx_conn_->prepare("whole_map_index", R"(SELECT id, external_id, name FROM map_index )");
    
    SPDLOG_TRACE(log, "ares::character::database::database done preparing statements");
  }
}

