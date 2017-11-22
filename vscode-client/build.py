#!/usr/bin/env python

import subprocess
import sys

if __name__ == "__main__":
    vsce = 'vsce.cmd' if sys.platform == 'win32' else 'vsce'
    sys.exit(subprocess.call([vsce, 'package', '-o', '../vscode-extension.vsix']))
