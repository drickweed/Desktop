# Tags
create table if not exists tags (id INTEGER PRIMARY KEY, name STRING);

# Tag assignment
create table if not exists tags_items (id INTEGER PRIMARY KEY, tag_id INTEGER, item_id INTEGER, item_table STRING);

# Make tags on each item unique
create unique index if not exists unique_item_tags on tags_items (tag_id, item_id, item_table);
