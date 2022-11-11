using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Collections;
using System.Drawing;
using System.Reflection;
using System.ComponentModel;

namespace sceneEditor.Configuration {

    public class ConfiguratorBase {
        SerializableOptionsCollection coll;
        string configFile;

        public ConfiguratorBase() { 
             coll = new SerializableOptionsCollection(this);
        }

        public SerializableOptionsCollection Options { get { return coll; } }
        public string File { get { return configFile; } set { configFile = value; } }

        public void LoadConfiguration(string fileName) {
            File = fileName;
            if (!System.IO.File.Exists(fileName)) return;
            Options.Clear();
            XmlReader reader = XmlReader.Create(fileName);
            try { reader.ReadStartElement("Configurator"); }
            catch (Exception e) {
                reader.Close();
                return;
            }
            while (reader.Name != "Configurator") {
                SerializableOptions opt = new SerializableOptions(reader.Name);
                try {
                    reader.ReadStartElement();
                    opt.LoadOptions(reader);
                    reader.ReadEndElement();
                    Options.Add(opt);
                }
                catch (XmlException) { 
                    reader.Close();
                    return;
                }
            }
            reader.ReadEndElement();
            reader.Close();
        }
        public void SaveConfiguration(string fileName) {
            XmlWriterSettings settings = new XmlWriterSettings();
            settings.ConformanceLevel = ConformanceLevel.Auto;
            XmlWriter writer = XmlWriter.Create(fileName, settings);
            writer.WriteStartElement("Configurator");
            foreach (SerializableOptions opt in Options) {
                opt.SaveOptions(writer);
            }
            writer.WriteEndElement();
            writer.Close();
        }

        public void SetValue(string optionsName, string name, object value) {
            if (Options[optionsName] == SerializableOptions.Empty) Options.Add(optionsName);
            Options[optionsName].SetValue(name, value);
        }
        public void SetAndSave(string optionsName, string name, object value) {
            SetValue(optionsName, name, value);
            SaveConfiguration(File);
        }

        public bool IsCustomXmlSerializableObject(Type tp) {
            object[] attr = tp.GetCustomAttributes(true);
            foreach(object obj in attr) {
                if (obj is CustomXmlSerializable) return true;
            }
            return false;
        }

        public bool IsCustomSerializableProperty(PropertyInfo p) {
            object[] attr = p.GetCustomAttributes(true);
            foreach (object obj in attr) {
                if (obj is CustomXmlSerializableProperty) return true;
            }
            return false;
        }

        public void SerializeAllProperties(string optionsName, object obj) {
            Type tp = obj.GetType();
            PropertyInfo[] pInfo = tp.GetProperties();
            bool isCustomSerializable = IsCustomXmlSerializableObject(tp);
            foreach (PropertyInfo prop in pInfo) {
                if (!prop.CanWrite || !prop.PropertyType.IsPublic || (isCustomSerializable && !IsCustomSerializableProperty(prop))) continue;
                SetValue(optionsName, prop.Name, prop.GetValue(obj,null));
            }
        }
        public void DeserializeAllProperties(string optionsName, object obj) {
            Type tp = obj.GetType();
            PropertyInfo[] pInfo = tp.GetProperties();
            foreach (PropertyInfo prop in pInfo) {
                if (!prop.CanWrite || !prop.PropertyType.IsPublic ) continue;
                prop.SetValue(obj, Options[optionsName].GetValue(prop.Name, prop.GetValue(obj, null)), null);
            }
        }
    }

    public class Configurator : ConfiguratorBase {
        public static Configurator Default = new Configurator();
        public Configurator() { }
    }

    public class EmptySerializableOptions : SerializableOptions {
        public EmptySerializableOptions(string name) : base(name) { }
        public override void SetValue(string name, object value) {
        }
        public override object GetValue(string name) {
            return null;
        }
        public override Dictionary<string, object> Values {
            get { return null; }
        }
    }

    public class SerializableOptions {
        public static SerializableOptions Empty = new EmptySerializableOptions("Empty");
        
        ConfiguratorBase owner;
        Dictionary<string, object> values;
        string name;

        public SerializableOptions(string name) {
            this.values = new Dictionary<string, object>();
            this.name = name;
        }

        public ConfiguratorBase Owner {
            get { return owner; }
            set { owner = value; }
        }

        protected internal virtual void SaveOptions(XmlWriter writer) {
            writer.WriteStartElement(Name);
            
            SizeConverter sc = new SizeConverter();
            PointConverter pc = new PointConverter();
            ColorConverter cc = new ColorConverter();

            foreach (string name in Values.Keys) {
                writer.WriteStartElement(name);
                writer.WriteAttributeString("type", Values[name].GetType().ToString());
                writer.WriteAttributeString("asm", Values[name].GetType().Assembly.ToString());
                if (Values[name] is Size)
                    writer.WriteRaw(sc.ConvertToInvariantString(Values[name]));
                else if (Values[name] is Point)
                    writer.WriteRaw(pc.ConvertToInvariantString(Values[name]));
                else if (Values[name] is Color)
                    writer.WriteRaw(cc.ConvertToInvariantString(Values[name]));
                else
                    WriteValue(writer, Values[name]);
                writer.WriteEndElement();
            }
            writer.WriteEndElement();
        }

        protected virtual void ReadValue(XmlReader reader, string asm, string type, string nm) {
            Assembly a = Assembly.Load(asm);
            Type tp = a.GetType(type);
            if (tp.IsEnum) {
                SetValue(nm, Enum.Parse(tp, reader.ReadElementContentAsString()));
                return;
            }
            object[] attr = tp.GetCustomAttributes(typeof(TypeConverterAttribute), true);
            if (attr.Length == 0) {
                reader.ReadElementContentAsString();
                return;
            }
            TypeConverterAttribute tca = attr[0] as TypeConverterAttribute;
            Type tct = Type.GetType(tca.ConverterTypeName);
            if (tct == null) {
                reader.ReadElementContentAsString();
                return;
            }
            TypeConverter tc = tct.GetConstructor(new Type[] { }).Invoke(null) as TypeConverter;
            SetValue(nm, tc.ConvertFromString(reader.ReadElementContentAsString()));
        }

        protected virtual void WriteValue(XmlWriter writer, object value) {
            Type tp = value.GetType();
            string asm = tp.Assembly.ToString();
            if (tp.IsEnum) {
                writer.WriteRaw(value.ToString());
                return;
            }
            object[] attr = tp.GetCustomAttributes(typeof(TypeConverterAttribute), true);
            if (attr.Length == 0) {
                writer.WriteRaw(value.ToString());
                return;
            }
            TypeConverterAttribute tca = attr[0] as TypeConverterAttribute;
            Type tct = Type.GetType(tca.ConverterTypeName);
            if (tct == null) {
                writer.WriteRaw(value.ToString());
                return;
            }
            TypeConverter tc = tct.GetConstructor(new Type[] {}).Invoke(null) as TypeConverter;
            writer.WriteRaw(tc.ConvertToString(value));
        }

        protected internal virtual void LoadOptions(XmlReader reader) { 
            Values.Clear();

            SizeConverter sc = new SizeConverter();
            PointConverter pc = new PointConverter();
            ColorConverter cc = new ColorConverter();

            while (reader.AttributeCount > 0) {
                try {
                    string type = reader.GetAttribute(0);
                    string asm = reader.GetAttribute(1);
                    string nm = reader.Name;
                    if (type == typeof(bool).ToString()) {
                        bool val = bool.Parse(reader.ReadElementContentAsString());
                        SetValue(nm, val);
                    }
                    else if (type == typeof(int).ToString()) {
                        int val = int.Parse(reader.ReadElementContentAsString());
                        SetValue(nm, val);
                    }
                    else if (type == typeof(float).ToString()) {
                        float val = float.Parse(reader.ReadElementContentAsString());
                        SetValue(nm, val);
                    }
                    else if (type == typeof(Size).ToString()) {
                        Size sz = (Size)sc.ConvertFromInvariantString(reader.ReadElementContentAsString());
                        SetValue(nm, sz);
                    }
                    else if (type == typeof(Point).ToString()) {
                        Point pt = (Point)pc.ConvertFromInvariantString(reader.ReadElementContentAsString());
                        SetValue(nm, pt);
                    }
                    else if (type == typeof(Color).ToString()) {
                        Color c = (Color)cc.ConvertFromInvariantString(reader.ReadElementContentAsString());
                        SetValue(nm, c);
                    }
                    else {
                        ReadValue(reader, asm, type, nm);
                    }
                }
                catch (XmlException) {
                    break;
                }
            }
        }

        public virtual Dictionary<string, object> Values { get { return values; } }
        public virtual object GetValue(string name) { 
            object outValue;
            if (Values.TryGetValue(name, out outValue)) return outValue;
            return null;
        }
        public virtual object GetValue(string name, object defaultValue) {
            object res = GetValue(name);
            if (res == null) return defaultValue;
            return res;
        }
        public virtual void SetValue(string name, object value) {
            Values[name] = value;
        }
        public void SetAndSave(string name, object value) {
            SetValue(name, value);
            if(Owner != null)
                Owner.SaveConfiguration(Owner.File);
        }

        public string Name { get { return name; } set { name = value; } }
    }

    public class SerializableOptionsCollection : CollectionBase {
        ConfiguratorBase conf;

        public SerializableOptionsCollection(ConfiguratorBase conf) {
            this.conf = conf;
        }

        public ConfiguratorBase Configurator { get { return conf; } }

        public int Add(SerializableOptions item) { return List.Add(item); }
        public void Insert(int index, SerializableOptions item) { List.Insert(index, item); }
        public void Remove(SerializableOptions item) { List.Remove(item); }
        public SerializableOptions this[int index] { get { return List[index] as SerializableOptions; } set { List[index] = value; } }
        public SerializableOptions this[string name] { 
            get {
                foreach (SerializableOptions item in this) {
                    if (item.Name == name) return item;
                }
                return SerializableOptions.Empty;
            } 
        }

        public int Add(string name) {
            if (this[name] != SerializableOptions.Empty) return IndexOf(this[name]);
            SerializableOptions opt = new SerializableOptions(name);
            return Add(opt);
        }

        public int IndexOf(SerializableOptions opt) { return List.IndexOf(opt); }
        protected override void OnInsertComplete(int index, object value) {
            base.OnInsertComplete(index, value);
            (value as SerializableOptions).Owner = Configurator;
        }
    }

    public class CustomXmlSerializable : System.Attribute {
        bool isCustomXmlSerializable;
        public CustomXmlSerializable(bool isCustomXmlSerializable) {
            this.isCustomXmlSerializable = isCustomXmlSerializable;
        }

        public bool IsCustomXmlSerializable { get { return isCustomXmlSerializable; } }
    }
    public class CustomXmlSerializableProperty : System.Attribute { 
        bool isCustomXmlSerializableProperty;
        public CustomXmlSerializableProperty(bool isCustomXmlSerializableProperty) {
            this.isCustomXmlSerializableProperty = isCustomXmlSerializableProperty;
        }
        public bool IsCustomXmlSerializableProperty { get { return isCustomXmlSerializableProperty; } }
    }
}
