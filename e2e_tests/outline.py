import e2e_test_runner
from e2e_tests.utils import *

def Test_Outline():
  return (e2e_test_runner.TestBuilder()
          .SetupCommonInit()
          .IndexFile("foo.cc",
                     """void foobar();""")
          .WaitForIdle()
          .Send({
              'id': 1,
              'method': 'textDocument/documentSymbol',
              'params': {
                  'textDocument': {
                      'uri': BuildUri('foo.cc')
                  }
              }
          })
          .Expect({
              'id': 1,
              'result': [{
                  'containerName': 'void foobar()',
                  'kind': 12,
                  'name': 'foobar',
                  'location': {
                      'range': {
                          'start': {
                              'line': 0,
                              'character': 5
                          },
                          'end': {
                              'line': 0,
                              'character': 11
                          }
                      },
                      'uri': BuildUri('foo.cc')
                  }
              }]
          }))
