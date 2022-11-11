using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using Microsoft.Win32;
using System.Security.AccessControl;

namespace sceneEditor
{
    public class SceneFilesInfo
    {
        string sceneFile;
        string dataDir;
    
        public SceneFilesInfo() { }
        public SceneFilesInfo( string fileName, string dirName ) {
            this.sceneFile = fileName;
            this.dataDir = dirName;
        }

        public string SceneFile { get { return sceneFile; } set { sceneFile = value; } }
        public string DataDir { get { return dataDir; } set { dataDir = value; } }

        public override bool Equals(object obj)
        {
            SceneFilesInfo info = obj as SceneFilesInfo;
            if(info == null) return base.Equals(obj);
            return SceneFile == info.SceneFile && DataDir == info.DataDir;
        }
        public override int GetHashCode()
        {
            return SceneFile.GetHashCode() ^ DataDir.GetHashCode();
        }
    }

    public class SceneFilesInfoCollection : CollectionBase {
        int recentCount;
        
        public SceneFilesInfoCollection(int recentCount) {
            this.recentCount = recentCount;
        }
        public SceneFilesInfoCollection() : this(4) { }
        
        public int Add( SceneFilesInfo info ) { return List.Add(info); }
        public void Insert(int index, SceneFilesInfo info) { List.Insert(index, info); }
        public void Remove(SceneFilesInfo info) { List.Remove(info); }

        public SceneFilesInfo this[int index] { get { return List[index] as SceneFilesInfo; } set { List[index] = value; } }

        public bool Contains(SceneFilesInfo info) { 
            foreach(SceneFilesInfo info2 in this) {
                if (info.SceneFile == info2.SceneFile && info.DataDir == info2.DataDir) return true;
            }
            return false;
        }

        public int RecentCount { 
            get { return recentCount; } 
            set { recentCount = value; }
        }
            
        public void Serialize(string subKey) {
            using (RegistryKey key = Registry.CurrentUser.OpenSubKey("SOFTWARE", true))
            {
                using (RegistryKey key2 = key.CreateSubKey(subKey, RegistryKeyPermissionCheck.ReadWriteSubTree))
                {
                    key2.SetValue("RecentCount", Math.Min(Count, RecentCount), RegistryValueKind.DWord);

                    for (int i = 0; i < Math.Min(Count, RecentCount); i++)
                    {
                        key2.SetValue("sceneFile" + i.ToString(), this[i].SceneFile, RegistryValueKind.String);
                        key2.SetValue("dataDir" + i.ToString(), this[i].DataDir, RegistryValueKind.String);
                    }
                }
            }
        }

        public void DeSerialize(string subKey) {
            using(RegistryKey key = Registry.CurrentUser.OpenSubKey("SOFTWARE")) { 
                using(RegistryKey key2 = key.OpenSubKey(subKey, false)) {
                    if (key2 == null) return;

                    int cnt = (int)key2.GetValue("RecentCount", 0);
                    for(int i = 0; i<cnt; i++) {
                        string val1 = (string)key2.GetValue("sceneFile" + i.ToString(), string.Empty);
                        string val2 = (string)key2.GetValue("dataDir" + i.ToString(), string.Empty);

                        if (val1 == string.Empty || val2 == string.Empty) continue;
                        Add(new SceneFilesInfo(val1, val2));
                    }
                }
            }
        }
    }
}
