CREATE SCHEMA IF NOT EXISTS short_link_schema;

CREATE TABLE IF NOT EXISTS short_link_schema.links (
    code VARCHAR(8) PRIMARY KEY,
    original_url TEXT NOT NULL,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    expires_at TIMESTAMPTZ NOT NULL,
    clicks INT DEFAULT 0
);