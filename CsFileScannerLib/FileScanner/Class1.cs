using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using RGiesecke.DllExport;

namespace FileScannerLibrary
{
    public class FileScanner
    {
        private static string SystemDrive;

        static FileScanner()
        {
            // Dynamically determine the Windows drive
            string windowsPath = Environment.GetFolderPath(Environment.SpecialFolder.Windows);
            SystemDrive = Path.GetPathRoot(windowsPath);

            // Define system-critical directories
            SystemCriticalPaths = new string[]
            {
                Path.Combine(SystemDrive, "Windows"),
                Path.Combine(SystemDrive, "Program Files"),
                Path.Combine(SystemDrive, "Program Files (x86)"),
                Path.Combine(SystemDrive, "ProgramData"),
                Path.Combine(SystemDrive, @"Users\Default")
            };
        }

        private static readonly string[] SystemCriticalPaths;

        [DllExport("ScanDirectory", CallingConvention = CallingConvention.StdCall)]
        public static IntPtr ScanDirectory(IntPtr directoryPathPtr)
        {
            string directoryPath = MarshalExtensions.PtrToStringUTF8(directoryPathPtr);

            if (Directory.Exists(directoryPath))
            {
                try
                {
                    string[] files = Directory.GetFiles(directoryPath, "*.*", SearchOption.AllDirectories);
                    var filteredFiles = new System.Collections.Generic.List<string>();

                    foreach (string file in files)
                    {
                        if (!IsInSystemCriticalPath(file))
                        {
                            filteredFiles.Add(file);
                        }
                    }

                    string result = string.Join("|", filteredFiles);
                    return MarshalExtensions.StringToHGlobalUTF8(result);
                }
                catch (Exception ex)
                {
                    Console.WriteLine("Error scanning directory: " + ex.Message);
                    return IntPtr.Zero;
                }
            }

            return IntPtr.Zero;
        }

        [DllExport("FreeMemory", CallingConvention = CallingConvention.StdCall)]
        public static void FreeMemory(IntPtr ptr)
        {
            Marshal.FreeHGlobal(ptr);
        }

        private static bool IsInSystemCriticalPath(string filePath)
        {
            foreach (string criticalPath in SystemCriticalPaths)
            {
                if (filePath.StartsWith(criticalPath, StringComparison.OrdinalIgnoreCase))
                {
                    return true;
                }
            }
            return false;
        }
    }

    public static class MarshalExtensions
    {
        public static string PtrToStringUTF8(IntPtr ptr)
        {
            if (ptr == IntPtr.Zero)
                return null;

            int len = 0;
            while (Marshal.ReadByte(ptr, len) != 0)
                ++len;

            byte[] buffer = new byte[len];
            Marshal.Copy(ptr, buffer, 0, buffer.Length);
            return Encoding.UTF8.GetString(buffer);
        }

        public static IntPtr StringToHGlobalUTF8(string str)
        {
            if (str == null)
                return IntPtr.Zero;

            byte[] bytes = Encoding.UTF8.GetBytes(str);
            IntPtr ptr = Marshal.AllocHGlobal(bytes.Length + 1);
            Marshal.Copy(bytes, 0, ptr, bytes.Length);
            Marshal.WriteByte(ptr, bytes.Length, 0);
            return ptr;
        }
    }
}