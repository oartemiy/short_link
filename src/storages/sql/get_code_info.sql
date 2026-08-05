SELECT
    original_url,
    created_at,
    expires_at,
    clicks
FROM
    short_link_schema.links
WHERE
    code = $1
