using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.ShaderParams {
    public partial class ShaderParamsControl : ControlWithSceneUpdate {
        public ShaderParamsControl() {
            InitializeComponent();
        }
    
        
        a3dShaderParamBaseMapperCollection paramCollection;
        public a3dShaderParamBaseMapperCollection ParamCollection {
            get { return paramCollection; }
            set {
                paramCollection = value;
                OnParamCollectionChanged();
            }
        }

        protected virtual void OnParamCollectionChanged() {
            Controls.Clear();
            if(ParamCollection == null) return;
            foreach(a3dShaderParamBaseMapper param in ParamCollection) {
                ShaderParamInfoBase info = CreateShaderParam(param);
                info.MainForm = MainForm;
                info.Param = param;
                Controls.Add(info);
            }
            LayoutControl();
        }

        protected virtual void LayoutControl() {
            Point loc = new Point(2, 2);

            int width = 0;
            foreach(ShaderParamInfoBase paramInfo in Controls) {
                width = Math.Max(width, paramInfo.Width);
            }
            
            foreach(ShaderParamInfoBase paramInfo in Controls) {
                paramInfo.Location = loc;
                paramInfo.Width = width;
                loc = new Point(2, paramInfo.Bounds.Bottom + 2);
            }
        }

        protected ShaderParamInfoBase CreateBoolArrayShaderParam(a3dShaderParamBaseMapper param) {
            if (param.ArrayDimension == 0) return new BoolMatrixShaderParam();
            if (param.Row == 1 && param.Col == 1) return new BoolArrayShaderParam();
            return new BoolMatrixArrayShaderParam();
        }

        protected ShaderParamInfoBase CreateIntArrayShaderParam(a3dShaderParamBaseMapper param) {
            if (param.ArrayDimension == 0) return new IntMatrixShaderParam();
            if (param.Row == 1 && param.Col == 1) return new IntArrayShaderParam();
            return new IntMatrixArrayShaderParam();
        }

        protected ShaderParamInfoBase CreateFloatArrayShaderParam(a3dShaderParamBaseMapper param) {
            if (param.WidgetType == a3dShaderParamWidgetTypeMapper.SpwtColor) return new ColorShaderParam();
            if (param.ArrayDimension == 0) return new FloatMatrixShaderParam();
            if (param.Row == 1 && param.Col == 1) return new FloatArrayShaderParam();
            return new FloatMatrixArrayShaderParam();
        }

        protected virtual ShaderParamInfoBase CreateShaderParam(a3dShaderParamBaseMapper param) {
            switch(param.Type) {
                case a3dShaderParamTypeMapper.SptBase: return new ShaderParamInfoBase();
                case a3dShaderParamTypeMapper.SptBool: return new BoolShaderParam();
                case a3dShaderParamTypeMapper.SptBoolArray: return CreateBoolArrayShaderParam(param);
                case a3dShaderParamTypeMapper.SptInt: return new IntShaderParam();
                case a3dShaderParamTypeMapper.SptIntArray: return CreateIntArrayShaderParam(param);
                case a3dShaderParamTypeMapper.SptFloat: return new FloatShaderParam();
                case a3dShaderParamTypeMapper.SptFloatArray:
                case a3dShaderParamTypeMapper.SptVector:
                case a3dShaderParamTypeMapper.SptMatrix:
                case a3dShaderParamTypeMapper.SptMatrixArray: return CreateFloatArrayShaderParam(param);
                case a3dShaderParamTypeMapper.SptTechnique: return new TechniqueShaderParam();
                case a3dShaderParamTypeMapper.SptTexture: return new TextureShaderParam();
            }
            return new ShaderParamInfoBase();
        }
    }
}
