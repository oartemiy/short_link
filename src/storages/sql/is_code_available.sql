SELECT
    code
FROM
    short_link_schema.links
WHERE
    code = $1
