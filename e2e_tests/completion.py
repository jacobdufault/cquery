import e2e_test_runner
from e2e_tests.utils import *

def IGNORE_Test_Completion():
  return (e2e_test_runner.TestBuilder()
          .SetupCommonInit()
          .IndexFile("foo.cc",
                     """
struct Foo {
  int aaa;
};
void foobar() {
  Foo f;
  f.a
}""")
          .WaitForIdle()
          .SendDidOpen('foo.cc')
          .Send({
              'id': 1,
              'method': 'textDocument/completion',
              'params': {
                  'textDocument': {
                      'uri': BuildUri('foo.cc')
                  },
                  'position': {
                    'line': 7,
                    'character': 5
                  }
              }
          })
          .Expect({
              'id': 1,
              'result': [{
              }]
          }))
