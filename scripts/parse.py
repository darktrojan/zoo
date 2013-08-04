#!/usr/bin/python
import json
import sys
import Parser

path = sys.argv[1]

p = Parser.getParser(path)
p.readFile(path)

j = []
for e in p:
	if isinstance(e, Parser.Junk):
		j.append(e.get_all())
	else:
		j.append({
			'pre_ws': e.get_pre_ws(),
			'pre_comment': e.get_pre_comment(),
			'key': e.get_key(),
			'value': e.get_val(),
			'post': e.get_post()
		})
	# print '%s%s%s%s' % (e.get_pre_ws(), e.get_pre_comment(), a, e.get_post()),

	# def get_all(self):
	#   return self.contents[self.span[0] : self.span[1]]
	# def get_pre_ws(self):
	#   return self.contents[self.pre_ws_span[0] : self.pre_ws_span[1]]
	# def get_pre_comment(self):
	#   return self.contents[self.pre_comment_span[0] : self.pre_comment_span[1]]
	# def get_def(self):
	#   return self.contents[self.def_span[0] : self.def_span[1]]
	# def get_key(self):
	#   return self.contents[self.key_span[0] : self.key_span[1]]
	# def get_val(self):
	#   return self.pp(self.contents[self.val_span[0] : self.val_span[1]])
	# def get_post(self):

print json.dumps(j)
