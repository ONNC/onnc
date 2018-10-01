ONNC is a “release often” project. To keep up with the development or share your ideas, 
please join [mailing list](https://maillist.onnc.ai/) now! now!

#### How to contribute your code (in sequence)

1. Ensure your idea have discussed in [mailing list](https://maillist.onnc.ai/).
1. Post your summary and implementation plan in [Issues](https://github.com/ONNC/onnc/issues).
1. Implement and run [unittest](https://github.com/ONNC/onnc/CONTRIBUTING.md#running-unittests).
1. Create Pull Request, then report PR number to corresponding mailing list or Issues.

#### Running unittests

1. Build with debug mode, take onnc-umbrella as an example:
  * $> git clone --recursive https://github.com/ONNC/onnc-umbrella.git ${ONNC}
  * $> cd ${ONNC} && ./build.sh dbg ${TARGET_TO_INSTALL}
2. You will have a executable file named "unittest" in build-dbg/tools.
3. Run "unittest", and get all pass.
