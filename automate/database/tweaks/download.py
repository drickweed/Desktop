import urllib2, re

local = True
fh = open("../tweaks.xml")
lines = fh.readlines()
for line in lines:
	match = re.search('link="([^"]*)', line)
	try:
		url = match.groups()[0]
		if not url.startswith("http"):
			local = True
			url = "http://www.kellys-korner-xp.com/%s" % url
		else:
			local = False
		url = url.replace("&amp;", "&")
		print url

	except:
		pass
