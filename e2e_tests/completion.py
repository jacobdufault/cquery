import e2e_test_runner


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
                      'uri': 'foo.cc'
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
