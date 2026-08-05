DELETE FROM short_link_schema.links
WHERE code = $1
    AND expires_at > NOW()
RETURNING
    code
