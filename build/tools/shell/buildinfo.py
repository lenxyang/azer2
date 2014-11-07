import os;
import sys;
import time;
import socket;

if __name__ == "__main__":
    gen_dir = sys.argv[1];
    buildinfo_file = sys.argv[2]
    absolute_build_path = os.path.normpath(sys.argv[3]);
    path_encoded = absolute_build_path.encode("string_escape");
    if (not os.path.exists(os.path.dirname(gen_dir))):
        os.makedirs(os.path.dirname(gen_dir))

    lt = time.localtime();
    build_machine = socket.gethostname();
    build_time = time.strftime('%Y-%m-%d %H:%M:%S',time.localtime());
    f = open("%s/%s.cc" % ( gen_dir, buildinfo_file), "w");
    f.write("#include \"base/files/file_path.h\"\n\n");
    f.write("namespace azer {\n\n");
    f.write("extern const int kBuildRootDirLength = %d;\n" % (len(absolute_build_path)));
    f.write("extern const ::base::FilePath::StringType kBuildRootDir = FILE_PATH_LITERAL(\"%s\");\n" % (path_encoded));
    f.write("extern const char* kBuildTime=\"%s\";\n" % (build_time))
    f.write("extern const char* kBuildMachine=\"%s\";\n" % (build_machine))
    f.write("}  // namespace azer\n\n")
    
