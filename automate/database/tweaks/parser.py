
import re, sys
fh = open("cleanup.html")
target = open("tweaks.xml", 'w')
target.write("<tweaks>\n")
lines = fh.readlines()
for line in lines:
	regex_links = re.findall('href="([^"]*)"[^>]*>([^<]*)', line)
	regex_text = re.findall('<br/>([^<]*)', line)
	target.write("  <tweak>\n")
	d = dict()
	[d.__setitem__(key, value) for (key, value) in regex_links]
	for (key, value) in d.items():
		target.write("    <action link=\"%s\">%s</action>\n" % (key, value))
	if regex_text is not None and len(regex_text)>0:
		target.write("    <description>\n")
		[target.write(s) for s in regex_text]
		target.write("    </description>\n")
	target.write("  </tweak>\n")
target.write("</tweaks>\n")
target.close()
