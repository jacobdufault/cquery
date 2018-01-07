#!/usr/bin/python

import json
import os
import importlib
import re
import shutil
import subprocess

CQUERY_PATH = 'build/release/bin/cquery'
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
                '-xc++',
                '-std=c++11'
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
    cquery will pause processing messages until it is idle.
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
                    'triggerCharacters': ['.', ':', '>', '#']
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
                    'resolveProvider': False
                }
            }
        }
    })
    return self


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
  actual = GetMessages(stdout.decode('utf8'))
  success = exit_code == 0 and actual == test_builder.expected

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
