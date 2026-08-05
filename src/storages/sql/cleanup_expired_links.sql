DELETE FROM short_link_schema.links
WHERE expires_at <= NOW()
