using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BetweennessGraphManipulator
{
    class Program
    {
        static void Main(string[] args)
        {
            bool generateInputFiles = true;
            bool generateFilesForVisualization = false;
            string foldersPath = @"Graphs";//folder path that contains folders OpenStreetMaps, BetweennessInputs, BetweennessOutputs, VisualizationInputs

            for (int i = 0; i < args.Length; i++)
            {
                if (args[i] == "-help" || args[i] == "-h")
                {
                    PrintHelp();
                    Environment.Exit(0);
                }
                else
                {
                    if (i + 1 == args.Length)
                    {
                        PrintHelp();
                        Environment.Exit(0);
                    }

                    if (args[i] == "-f")
                    {
                        foldersPath = args[i + 1];
                    }
                    else if (args[i] == "-o")
                    {
                        int operation = int.Parse(args[i + 1]);
                        if (operation == 1)
                        {
                            generateInputFiles = true;
                            generateFilesForVisualization = false;
                        }
                        else
                        {
                            generateInputFiles = false;
                            generateFilesForVisualization = true;
                        }
                    }
                    i++;
                }
            }
            Console.WriteLine($"Generate input files for BC algorithm: {generateInputFiles}");
            Console.WriteLine($"Generate input files for visualization: {generateFilesForVisualization}");
            Console.WriteLine($"Folder path: {foldersPath}");
            Console.WriteLine();

            string openStreetMapsFolderName = "OpenStreetMaps";//files generatet from openstreetmap in format vertex1;vertex2;distance;edge_id
            string betweennessInputsFolderName = "BetweennessInputs";//generated from openstreetmaps folder
            string betweennessOutputsFolderName = "BetweennessOutputs";
            string visualizationInputsFolderName = "VisualizationInputs";//generated from openstreetmaps and betweennessoutputs folders
            
            //1. GENERATE INPUT FILES FOR BETWEENNESS FROM OPEN STREET MAPS FILES
            if (generateInputFiles)
            {
                Console.WriteLine("Generating input files for betweenness algorithm...");
                string[] files = Directory.GetFiles(Path.Combine(foldersPath, openStreetMapsFolderName));

                foreach (var filePath in files)
                {
                    string extension = Path.GetExtension(filePath);
                    string fileName = Path.GetFileName(filePath);
                    string editedFileName = Path.GetFileNameWithoutExtension(fileName) + "_preprocessed" + extension;

                    var lines = ReadOpenStreetMapFormat(filePath);

                    string outputFolderPath = Path.Combine(foldersPath, betweennessInputsFolderName);
                    WriteToBetweennessInputFormat(lines, Path.Combine(outputFolderPath, editedFileName));
                }
                Console.WriteLine("Files generated successfully");
            }

            //2. CALCULATE BETWEENNESS ON BETWEENNESS INPUTS FILES... and put resulted edge betweenness to BETWEENNES OUTPUTS FOLDER

            //3. COMBINE OPENSTREET MAP FILES WITH BETWEENNESS OUTPUTS FILE FOR VISUALIZATION
            if (generateFilesForVisualization)
            {
                Console.WriteLine("Generating input files for visualization...");
                string[] inputFiles = Directory.GetFiles(Path.Combine(foldersPath, openStreetMapsFolderName));
                string[] bwOutputFiles = Directory.GetFiles(Path.Combine(foldersPath, betweennessOutputsFolderName));
                int fileCount = bwOutputFiles.Length;

                for (int i = 0; i < fileCount; i++)
                {
                    string inputFileName = Path.GetFileName(bwOutputFiles[i]).Replace("_preprocessed.csv_result_edge_betweenness", "");
                    string inputFilePath = Path.Combine(Path.Combine(foldersPath, openStreetMapsFolderName), inputFileName);
                    string visualizationFileName = Path.GetFileNameWithoutExtension(inputFilePath) + "_for_visualization" + Path.GetExtension(inputFilePath);
                    string visualizationFilePath = Path.Combine(Path.Combine(foldersPath, visualizationInputsFolderName), visualizationFileName);
                    CombineOpenStreetMapFilWithEdgeBetweennessResult(inputFilePath, bwOutputFiles[i], visualizationFilePath);
                }
                Console.WriteLine("Files generated successfully");
            }

            //4. VISUALIZE
        }

        private static void PrintHelp()
        {
            Console.WriteLine();
            Console.WriteLine("Help for graph data manipulator for betweenness centrality (BC) algorithm");
            Console.WriteLine("Usage: BetweennessGraphManipulator.exe -f <folder> -o <operation>");
            Console.WriteLine();
            Console.WriteLine("Where:");
            Console.WriteLine("<folder>\tFolder containing folders OpenStreetMaps, BetweennessInputs, BetweennessOutputs, VisualizationInputs");
            Console.WriteLine("<operation>\tUse 1 for generation input to BC algorithm, 2 for generating inputs for visualization from BC results");
            Console.WriteLine();
            Console.WriteLine("Example:");
            Console.WriteLine(@"BetweennessGraphManipulator.exe -f C: \Users\jirih\Desktop\Grafy\ -o 1");
            Console.WriteLine();
        }

        private static void CombineOpenStreetMapFilWithEdgeBetweennessResult(string openStreetMapFile, string edgeBetweennessFile, string combinedFile)
        {
            using (var originalStream = File.OpenRead(openStreetMapFile))
            using (var betweennessStream = File.OpenRead(edgeBetweennessFile))
            using (var combinedStream = File.Open(combinedFile, FileMode.Create))
            {
                using (var oReader = new StreamReader(originalStream))
                using (var bReader = new StreamReader(betweennessStream))
                using (var cWriter = new StreamWriter(combinedStream))
                {
                    //ORIGINAL FILE FORMAT
                    //id1;id2;dist;edge_id
                    //25766365;25766367;516;545454
                    //...

                    //EDGE BETWEENNESS RESULT FILE
                    //ID;VALUE
                    //0;1883751.0000
                    //...

                    //RESULT FILE FOR VISUALIZATION
                    //id1;id2;dist;edge_id;VALUE
                    //25766365;25766367;516;545454;1883751.0000
                    //...

                    string line = oReader.ReadLine();
                    bReader.ReadLine();

                    cWriter.WriteLine(line + ";VALUE");

                    while ((line = oReader.ReadLine()) != null)
                    {
                        string line2 = bReader.ReadLine();
                        string[] values = line2.Split(';');

                        cWriter.WriteLine(line + ";" + values[1]);
                    }
                }
            }
        }

        private static List<string> ReadOpenStreetMapFormat(string fileName)
        {
            int normValue = 0;
            List<string> lines = new List<string>();

            using (var stream = File.OpenRead(fileName))
            {
                int numberOfvertices = 0;
                using (var reader = new StreamReader(stream))
                {
                    reader.ReadLine();//id1;id2;dist;edge_id reads first line, just desciption

                    //Read first line and get norm value because vertex ids should start from zero for BC algorithm
                    string line;
                    if ((line = reader.ReadLine()) != null)
                    {
                        string[] values = line.Split(';');
                        int first = int.Parse(values[0]);
                        normValue = first;
                        first = 0;
                        int second = int.Parse(values[1]) - normValue;
                        lines.Add($"{first} {second} {values[2]}");//;{parts[0]}, {parts[1]}");
                                                                   //Console.WriteLine(line);

                        while ((line = reader.ReadLine()) != null)
                        {
                            values = line.Split(';');
                            first = int.Parse(values[0]) - normValue;
                            second = int.Parse(values[1]) - normValue;
                            
                            lines.Add($"{first} {second} {values[2]}");
                        }
                        numberOfvertices = first + 1; //first value (input vertex to edge) at the end of file is number of vertices - 1 
                    }

                    lines.Insert(0, numberOfvertices.ToString()); //betweenness require number of vertices on the first line
                }
            }
            return lines;
        }

        private static void WriteToBetweennessInputFormat(List<string> lines, string fileName)
        {
            using (var stream = File.Open(fileName, FileMode.Create))
            {
                using (var writer = new StreamWriter(stream))
                {
                    foreach (var line in lines)
                    {
                        writer.WriteLine(line);
                    }
                }
            }
        }
    }
}
