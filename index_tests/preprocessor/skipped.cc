
#ifdef FOOBAR
void hello();
#endif

#if false



#endif

#if defined(OS_FOO)

#endif

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": ["2:0-4:7", "6:0-10:7", "12:0-14:7"],
  "types": [],
  "funcs": [],
  "vars": []
}
*/
