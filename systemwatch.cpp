//////////////////////////////////////////////////////////////////////////////////
// Course: SYSC4001
// Engineer: qiyang xia
// Engineer id:100837661
// Create Date:    2016/12/5 
// Design Name:    systemwatch
// Module Name:    CDBoost
// Project Name:   teamproject
// Target Devices: 
// Tool versions:   linux,DEV
//
//////////////////////////////////////////////////////////////////////////////////

using System;
using System.IO;
using System.Collections;
using System.Threading;
 
namespace watcher
{
 
    public class FileSystemWather
    {
        private FileSystemWatcher fsWather;
        private Hashtable hstbWather;
        public event FileSystemEventHandler OnChanged;
        public event FileSystemEventHandler OnCreated;
 

        public FileSystemWather(string path, string filter)
        {
            if (!Directory.Exists(path))
            {
                throw new Exception("cannot find path£º" + path);
            }
 
            hstbWather = new Hashtable();
 
            fsWather = new FileSystemWatcher(path);
            fsWather.IncludeSubdirectories = false;
            fsWather.Filter = filter;
            fsWather.Changed += new FileSystemEventHandler(fsWather_Changed);
            fsWather.Created += new FileSystemEventHandler(fsWather_Created);

        }
 
 
        private void fsWather_Created(object sender, FileSystemEventArgs e)
        {
            lock (hstbWather)
            {
                hstbWather.Add(e.FullPath, e);
            }
            WatcherProcess watcherProcess = new WatcherProcess(sender, e);
            watcherProcess.OnCompleted += new Completed(WatcherProcess_OnCompleted);
            watcherProcess.OnCreated += new FileSystemEventHandler(WatcherProcess_OnCreated);
            Thread threadDeal = new Thread(watcherProcess.Process);
            threadDeal.Start();
        }
 
        private void WatcherProcess_OnCreated(object sender, FileSystemEventArgs e)
        {
            OnCreated(sender, e);
        }
 
 
        private void fsWather_Changed(object sender, FileSystemEventArgs e)
        {
            if (e.ChangeType == WatcherChangeTypes.Changed)
            {
                if (hstbWather.ContainsKey(e.FullPath))
                {
                    WatcherChangeTypes oldType = ((FileSystemEventArgs)hstbWather[e.FullPath]).ChangeType;
                    if (oldType == WatcherChangeTypes.Created || oldType == WatcherChangeTypes.Changed)
                    {
                        return;
                    }
                }
            }
 
            lock (hstbWather)
            {
                hstbWather.Add(e.FullPath, e);
            }
            WatcherProcess watcherProcess = new WatcherProcess(sender, e);
            watcherProcess.OnCompleted += new Completed(WatcherProcess_OnCompleted);
            watcherProcess.OnChanged += new FileSystemEventHandler(WatcherProcess_OnChanged);
            Thread thread = new Thread(watcherProcess.Process);
            thread.Start();
        }
 
        private void WatcherProcess_OnChanged(object sender, FileSystemEventArgs e)
        {
            OnChanged(sender, e);
        }
        
        public void Start()
        {
            fsWather.EnableRaisingEvents = true;
        }
 

        public void Stop()
        {
            fsWather.EnableRaisingEvents = false;
        }
 

 
    }
}
