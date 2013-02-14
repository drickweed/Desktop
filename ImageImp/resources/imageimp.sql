# Content
CREATE TABLE groups (id INTEGER PRIMARY KEY, name TEXT NOT NULL, parent_id INTEGER)
CREATE TABLE albums (id INTEGER PRIMARY KEY, name TEXT NOT NULL, group_id INTEGER, last_image_path TEXT)
CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT NOT NULL, sid TEXT, group_id INTEGER)
CREATE TABLE photodates (id INTEGER PRIMARY KEY, datetime TEXT NOT NULL, album_id INTEGER, person_id INTEGER)
CREATE TABLE pictures (id INTEGER PRIMARY KEY, path TEXT NOT NULL, album_id INTEGER, person_id INTEGER)

# Selection
CREATE TABLE default_pictures (id INTEGER PRIMARY KEY, picture_id INTEGER)
CREATE TABLE album_templates (album_id INTEGER UNIQUE NOT NULL, template TEXT)
CREATE TABLE picture_templates (album_id INTEGER UNIQUE NOT NULL, template TEXT)

# Sort and Exclusion
CREATE TABLE excluded_people (item_id INTEGER UNIQUE NOT NULL)
CREATE TABLE ordered_people (item_id INTEGER UNIQUE NOT NULL, position INTEGER)

# Database cleanup triggers

# On removed picture

## Remove default picture for deleted picture
create trigger remove_default_picture after delete on pictures begin delete from default_pictures where old.id == picture_id; end

# On removed person

## Remove pictures for deleted person
create trigger remove_pictures_for_person after delete on people begin delete from pictures where old.id == person_id; end

## Remove photo dates for deleted person
create trigger remove_photodates_for_person after delete on people begin delete from photodates where old.id == person_id; end

## Remove order and exclusion for deleted person
create trigger remove_order_and_exclusion_for_person after delete on people begin delete from excluded_people where old.id == item_id; delete from ordered_people where old.id == item_id; end

# On removed album

## Remove pictures for deleted album
create trigger remove_pictures_for_album after delete on albums begin delete from pictures where old.id == album_id; end

## Remove photo dates for deleted album
create trigger remove_photodates_for_album after delete on albums begin delete from photodates where old.id == album_id; end

## Remove picture template for deleted album
create trigger remove_picture_template_for_album after delete on albums begin delete from picture_templates where old.id == album_id; end

## Remove album template for deleted album
create trigger remove_album_template_for_album after delete on albums begin delete from album_templates where old.id == album_id; end

# On removed group

## Remove people for deleted group
create trigger remove_people_for_group after delete on groups begin delete from people where old.id == group_id; end

## Remove albums for deleted group
create trigger remove_albums_for_group after delete on groups begin delete from albums where old.id == group_id; end