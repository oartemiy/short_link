INSERT INTO
    short_link_schema.links (
        code,
        original_url,
        created_at,
        expires_at
    )
VALUES (
        $1,
        $2,
        NOW(),
        NOW() + make_interval(secs => $3)
    )
RETURNING
    created_at,
    expires_at