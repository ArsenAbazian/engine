using System;
using System.Collections.Generic;
using System.Text;
using classMapper;
using sceneEditor.graphViewer;

namespace sceneEditor.InheritedClassMapper {
    
    class a3dScene : a3dSceneMapper {
        protected override a3dGraphMapper CreateGraphMapper() {
            return new Graph(false);
        }
    }
}
