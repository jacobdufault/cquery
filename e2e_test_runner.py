#!/usr/bin/python

import json
import os
import importlib
import re
import shutil
import subprocess

#CQUERY_PATH = 'build/release/bin/cquery'
CQUERY_PATH = 'x64/Debug/cquery.exe'
CACHE_DIR = 'e2e_cache'

# We write test files in python. The test runner collects all python files in
# the directory and executes them. The test function just creates a test object
# which specifies expected stdin/stdout.
#
# Test functions are automatically discovered; they just need to be in the
# global environment and start with `Test_`.


# If found in json output with quotes surrounding this value it will be replaced
# with null, ie, "~~null~~" -> null
NULL_MAGIC_VALUE = "~~null~~"

class TestBuilder:
  def __init__(self):
    self.sent = []
    self.expected = []
    self.documents = {}

  def IndexFile(self, path, contents):
    """
    Indexes the given file with contents.
    """
    self.documents[path] = contents
    self.Send({
        'method': '$cquery/indexFile',
        'params': {
            'path': path,
            'contents': contents,
            'args': [
                'clang',
                '-xc++',
                '-std=c++11',
                path
            ]
        }
    })
    return self

  def SendDidOpen(self, path):
    self.Send({
        'method': 'textDocument/didOpen',
        'params': {
            'textDocument': {
                'uri': path,
                'languageId': 'cpp',
                'version': 0,
                'text': self.documents[path]
            },
        }
    })
    return self

  def WaitForIdle(self):
    """
    cquery will wait until the import pipeline is idle
    """
    self.Send({'method': '$cquery/wait'})
    return self

  def Send(self, stdin):
    """
    Send the given message to the language server.
    """
    if not isinstance(stdin, str):
      stdin['jsonrpc'] = '2.0'
    self.sent.append(stdin)
    return self

  def Expect(self, expected):
    """
    Expect a message from the language server.
    """
    expected['jsonrpc'] = '2.0'
    self.expected.append(expected)
    return self

  def SetupCommonInit(self):
    """
    Add initialize/initialized messages.
    """
    self.Send({
        'id': 0,
        'method': 'initialize',
        'params': {
            'processId': 123,
            'rootUri': 'cquery',
            'capabilities': {
              'textDocument': {
                'codeLens': NULL_MAGIC_VALUE
              }
            },
            'trace': 'off',
            'initializationOptions': {
                'cacheDirectory': CACHE_DIR
            }
        }
    })
    self.Expect({
        'id': 0,
        'result': {
            'capabilities': {
                'textDocumentSync': 2,
                'hoverProvider': True,
                'completionProvider': {
                    'resolveProvider': False,
                    'triggerCharacters': ['.', ':', '>', '#', '<', '"', '/']
                },
                'signatureHelpProvider': {
                    'triggerCharacters': ['(', ',']
                },
                'definitionProvider': True,
                'referencesProvider': True,
                'documentHighlightProvider': True,
                'documentSymbolProvider': True,
                'workspaceSymbolProvider': True,
                'codeActionProvider': True,
                'codeLensProvider': {
                    'resolveProvider': False
                },
                'documentFormattingProvider': False,
                'documentRangeFormattingProvider': False,
                'renameProvider': True,
                'documentLinkProvider': {
                    'resolveProvider': True
                },
                'executeCommandProvider': {
                    'commands': []
                }
            }
        }
    })
    return self

class Diff(object):
    def __init__(self, first, second, with_values=True, vice_versa=False):
        self.difference = []
        self.check(first, second, with_values=with_values)

        if vice_versa:
            self.check(second, first, with_values=with_values)

    def check(self, first, second, path='', with_values=False):
        if second != None:
            if not isinstance(first, type(second)):
                message = '%s- %s, %s' % (path, type(first), type(second))
                self.save_diff(message, TYPE)

        if isinstance(first, dict):
            for key in first:
                # the first part of path must not have trailing dot.
                if len(path) == 0:
                    new_path = key
                else:
                    new_path = "%s.%s" % (path, key)

                if isinstance(second, dict):
                    if key in second:
                        sec = second[key]
                    else:
                        #  there are key in the first, that is not presented in the second
                        self.save_diff(new_path, path)

                        # prevent further values checking.
                        sec = None

                    # recursive call
                    self.check(first[key], sec, path=new_path, with_values=with_values)
                else:
                    # second is not dict. every key from first goes to the difference
                    self.save_diff(new_path, 'path')
                    self.check(first[key], second, path=new_path, with_values=with_values)

        # if object is list, loop over it and check.
        elif isinstance(first, list):
            for (index, item) in enumerate(first):
                new_path = "%s[%s]" % (path, index)
                # try to get the same index from second
                sec = None
                if second != None:
                    try:
                        sec = second[index]
                    except (IndexError, KeyError):
                        # goes to difference
                        self.save_diff('%s - %s, %s' % (new_path, type(first), type(second)), 'type')

                # recursive call
                self.check(first[index], sec, path=new_path, with_values=with_values)

        # not list, not dict. check for equality (only if with_values is True) and return.
        else:
            if with_values and second != None:
                if first != second:
                    self.save_diff('%s - %s | %s' % (path, first, second), 'VALUE')
            return

    def save_diff(self, diff_message, type_):
        message = '%s: %s' % (type_, diff_message)
        if diff_message not in self.difference:
            self.difference.append(message)

def _ExecuteTest(name, func):
  """
  Executes a specific test.

  |func| must return a TestBuilder object.
  """

  # Delete cache directory.
  shutil.rmtree(CACHE_DIR, ignore_errors=True)

  test_builder = func()
  # if not isinstance(test_builder, TestBuilder):
  if not test_builder.__class__.__name__ == 'TestBuilder':
    raise Exception('%s does not return a TestBuilder instance' % name)

  # Add a final exit message.
  test_builder.WaitForIdle()
  test_builder.Send({'method': 'exit'})

  # Convert messages to a stdin byte array.
  stdin = ''
  for message in test_builder.sent:
    payload = message
    if not isinstance(payload, str):
      payload = json.dumps(message)
      payload = payload.replace('"' + NULL_MAGIC_VALUE + '"', 'null')
    wrapped = 'Content-Length: %s\r\n\r\n%s' % (len(payload), payload)
    stdin += wrapped
  stdin_bytes = stdin.encode(encoding='UTF-8')

  # Finds all messages in |string| by parsing Content-Length headers.
  def GetMessages(string):
    messages = []
    for match in re.finditer('Content-Length: (\d+)\r\n\r\n', string):
      start = match.span()[1]
      length = int(match.groups()[0])
      message = string[start:start + length]
      decoded = json.loads(message)
      # Do not report '$cquery/progress' messages.
      if 'method' in decoded and decoded['method'] == '$cquery/progress':
        continue
      # Do not report '$cquery/setInactiveRegions' messages.
      if 'method' in decoded and decoded['method'] == '$cquery/setInactiveRegions':
        continue
      # Do not report 'textDocument/publishDiagnostic' messages.
      if 'method' in decoded and decoded['method'] == 'textDocument/publishDiagnostics':
        continue

      messages.append(decoded)
    return messages

  # Utility method to print a byte array.
  def PrintByteArray(bytes):
    for line in bytes.split(b'\r\n'):
      print(line.decode('utf8'))

  # Execute program.
  cmd = [CQUERY_PATH, '--language-server', '--log-all-to-stderr']
  process = subprocess.Popen(
      cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  (stdout, stderr) = process.communicate(stdin_bytes)
  exit_code = process.wait()

  # Check if test succeeded.
  success = exit_code == 0
  actual = GetMessages(stdout.decode('utf8'))
  expected = test_builder.expected
  common_end = min(len(test_builder.expected), len(actual))
  for i in range(0, common_end):
    diff = Diff(actual[i], expected[i])
    if len(diff.difference) != 0:
      print('############')
      print('Actual')
      print(actual[i])
      print('Expected')
      print(expected[i])
      success = False
      for d in diff.difference:
        print(d)

  # Print failure messages.
  if success:
    print('== Passed %s with exit_code=%s ==' % (name, exit_code))
  else:
    print('== FAILED %s with exit_code=%s ==' % (name, exit_code))
    print('## STDIN:')
    for message in GetMessages(stdin):
      print(json.dumps(message, indent=True))
    if stdout:
      print('## STDOUT:')
      for message in GetMessages(stdout.decode('utf8')):
        print(json.dumps(message, indent=True))
    if stderr:
      print('## STDERR:')
      PrintByteArray(stderr)

    print('## Expected output')
    for message in test_builder.expected:
      print(message)
    print('## Actual output')
    for message in actual:
      print(message)
    print('## Difference')
    common_end = min(len(test_builder.expected), len(actual))
    for i in range(0, common_end):
      if test_builder.expected[i] != actual[i]:
        print('i=%s' % i)
        print('- Expected %s' % str(test_builder.expected[i]))
        print('- Actual %s' % str(actual[i]))
    for i in range(common_end, len(test_builder.expected)):
      print('Extra expected: %s' % str(test_builder.expected[i]))
    for i in range(common_end, len(actual)):
      print('Extra actual: %s' % str(actual[i]))


def _LoadAllModulesFromDir(dirname):
  # https://stackoverflow.com/a/1057765
  result = []
  for item in os.listdir(dirname):
    if item == '__init__.py' or item[-3:] != '.py':
      continue
    module_path = dirname + '.' + item[:-3]
    print('Importing ' + module_path)
    module = importlib.import_module(module_path)
    result.append(module)
  return result


def _DiscoverTests():
  """
  Discover and return all tests.
  """
  for module in _LoadAllModulesFromDir('e2e_tests'):
    for name, value in module.__dict__.items():
      if not callable(value):
        continue
      if not name.startswith('Test_'):
        continue
      yield (name, value)


def _RunTests():
  """
  Executes all tests.
  """
  for name, func in _DiscoverTests():
    _ExecuteTest(name, func)


if __name__ == '__main__':
  _RunTests()
