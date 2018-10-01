ONNC is a “release often” project. To keep up with the development or share your ideas, 
please join [mailing list](https://maillist.onnc.ai/) now! now!

#### How to contribute your code (in sequence)

# Ensure your idea have discussed in [mailing list](https://maillist.onnc.ai/).
# Post your summary and implementation plan in [Issues](https://github.com/ONNC/onnc/issues).
# Implement and run [unittest](https://github.com/ONNC/onnc/CONTRIBUTING.md#running-unittests).
# Create Pull Request, then report PR number to corresponding mailing list or Issues.

#### Running unittests

# Build with debug mode, take onnc-umbrella as an example:
  $> git clone --recursive https://github.com/ONNC/onnc-umbrella.git ${ONNC}
  $> cd ${ONNC} && ./build.sh dbg ${TARGET_TO_INSTALL}
# You will have a executable file named "unittest" in build-dbg/tools.
# Run "unittest", and get all pass.
