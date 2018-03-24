import os

def BuildUri(relative_path):
  absolute_path = os.path.join(os.getcwd(), relative_path)
  map = {
      ' ': "%20",
      '#': "%23",
      '$': "%24",
      '&': "%26",
      '(': "%28",
      ')': "%29",
      '+': "%2B",
      ',': "%2C",
      ':': '%3A',
      ';': "%3B",
      '?': "%3F",
      '@': "%40",
  }

  result = ""
  for c in absolute_path:
    if c in map:
      result += map[c]
    else:
      result += c

  return 'file:///' + result.replace('\\', '/')

