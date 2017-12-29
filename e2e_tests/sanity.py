import e2e_test_runner


def Test_Sanity():
  return (e2e_test_runner.TestBuilder()
          .SetupCommonInit())
