using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace sceneEditor.Controls {
    [ToolboxItem(true)]
    class FloatValueControl : ValueTextBox {
        [DefaultValue(0.0f)]
        public float Value {
            get {
                if (Text == "") return 0.0f;
                return float.Parse(Text); 
            }
            set { Text = value.ToString(); }
        }        
    }
}
