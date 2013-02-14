Installation templates.

See included files for examples.

Items in the templates are proccessed in the order they appear in.

Available items:

<install version="ver">app</install>
	Install 'app' version 'ver'.

<run>command</run>
	Run 'command' on client.

<include>template</include>
	Include 'template'.

<command>command</command>
	Run internal command

<service startup="mode">service</service>
	Manage service startup and settings.

<setting value="value">value-name</setting>
	Change setting value.

<update install="boolean">update-filename</update>
	Install update
	
<registry>file</registry>
	Import registry file from those in the registry folder.

all tags can have additional attributes:
	winlang="code": only process item if the language code matches the
	installed windows os.

	winver="version" only process for specified version of windows
