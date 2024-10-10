# Tokens
_token_:

- _keyword_
- _identifier_
- _constant_
- _punctuator_
- _comment_ (to be discarded after scanning)

_keyword_:
- One of __char__, __else__, __for__, __hop__, __if__, __int__, __while__, __string__

_identifier_:
- any string matching the following regex that is not part of the above list: [A-Za-z_][A-Za-z0-9_]*

_constant_:
- any string matching the following regex: [1-9][0-9]*

_punctuator_:
- One of __(__, __)__, __{__, __}__, __++__, __--__, __&&__, __||__, __!__, __+__, __-__, __*__, __/__, __%__, __<__, __>__, __<=__, __>=__, __==__, __!=__, __=__, __,__, __;__, __'__, __"__

_comment_:
- any string matching either of the following regexes:
 !!.* or !\\-.*\\-!
 