UPDATE short_link_schema.links
SET
    clicks = clicks + 1
WHERE
    code = $1
    AND expires_at > NOW() RETURNING original_url