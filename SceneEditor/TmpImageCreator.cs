using System;
using System.Collections.Generic;
using System.Text;
using ddsConvert;

namespace sceneEditor {
    class TmpImageCreator {
        public static bool CreateTmpImage(string inFile, out string outFile) {
            outFile = "";
            if (!inFile.ToLower().Contains(".dds")) return false;
            outFile = inFile + ".bmp";
            ddsConverter.Default.convert(inFile, outFile);
            return true;
        }
    }
}
