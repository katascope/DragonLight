using System;
using System;
using System.Collections.Generic;
using InTheHand.Net.Sockets;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Threading;
using NAudio;
using NAudio.Wave;
using NAudio.MediaFoundation;
using NAudio.FileFormats;
using System.Windows.Threading;
using System.Windows.Controls.Primitives;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Threading.Channels;

namespace KataTracks
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        static string songName = "Unknown";
        static string JoystickToBle1 = "";
        static string JoystickToBle2 = "";
        static string songFilename = "";//..\\..\\..\\TronGame.m4a";
        static string imageFilename = "";//..\\..\\..\\TronGameCroppedCyan.png";
        static string configFilename = "..\\..\\..\\..\\..\\TronGame.TrackConfig";
        static WaveOutEvent outputDevice = null;
        static DispatcherTimer animationTimer;
        static DispatcherTimer connectionTimer;
        static DispatcherTimer btTextTimer;
        static DispatcherTimer btVolumeTimer;
        static bool playing = false;
        static DateTime literalTrackStartTime;
        float volume = 100;
        float boostVolume = 1.0f;
        static long timePick = 0;
        static bool b1Down = false;
        static ulong textTickCount = 0;
        static Thread discoverBleThread;
        static Dictionary<string, string> foundDevices = null;
        static bool useSoundTrigger = false;
        static float InputVolumeBias = 100;
        static float lastInputVolume = 0.0f;
        static int VolumeThreshold = 50;
        static GameController gameController = new GameController();

        public MainWindow()
        {
            InitializeComponent();
            MainLog.Text = "KataTracks initializing\n";

            outputDevice = new WaveOutEvent();
            if (outputDevice == null)
            {
                MainLog.Text += "No wave out\n";
            }

            animationTimer = new DispatcherTimer();
            connectionTimer = new DispatcherTimer();
            btTextTimer = new DispatcherTimer();
            btVolumeTimer = new DispatcherTimer();
            Canvas.SetLeft(TrackIndex, 0);
            Canvas.SetLeft(TrackIndexPlay, 0);
            
            InputVolumeSlider.Value = InputVolumeBias;

            gameController.Initialize();

            //CombinedBluetoothController.Initialize();
            LoadConfig(configFilename);

            DeviceVolume.Use("Line");
            DeviceWatcher.StartMonitoring();

            MainLog.Text += "Ready to Connect\n";

            animationTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            animationTimer.Interval = new TimeSpan(0, 0, 0, 0, 100);
            animationTimer.Start();

            btTextTimer.Tick += new EventHandler(btTextTimer_Tick);
            btTextTimer.Interval = new TimeSpan(0, 0, 0, 0, 200);
            btTextTimer.Start();

            btVolumeTimer.Tick += new EventHandler(btVolumeTimer_Tick);
            btVolumeTimer.Interval = new TimeSpan(0, 0, 0, 0, 100);
            btVolumeTimer.Start();

            EventManager.RegisterClassHandler(typeof(Window),
                 Keyboard.KeyUpEvent, new KeyEventHandler(OnButtonKeyUp), true);
        }

        private void LoadImage(string filename)
        {
            return;
            // Create image element to set as icon on the menu element
            BitmapImage bmImage = new BitmapImage();
            bmImage.BeginInit();
            bmImage.UriSource = new Uri(filename, UriKind.Absolute);
            bmImage.EndInit();
            //tronimage.Source = bmImage;
        }

        private void LoadConfig(string filename)
        {
            foreach (string line in System.IO.File.ReadLines(filename))
            {
                string[] splits = line.Split('=');
                if (splits.Length > 1)
                {
                    string key = splits[0]; //get key name
                    string value = splits[1].Split('#')[0].Trim();//remove comments

                    if (key == "SongName")
                        songName = value;
                    if (key == "SongFile")
                        songFilename = value;
                    if (key == "ImageFile")
                    {
                        imageFilename = value;
                        LoadImage(imageFilename);
                    }
                    if (key == "DeviceId")
                        DeviceManagerBLE.connectionList.Add(value);

                    if (key == "JoystickToBle1")
                        JoystickToBle1 = value;

                    if (key == "JoystickToBle2")
                        JoystickToBle2 = value;
                }
            }
        }

        private void PlayTrack(int seconds)
        {
            if (playing)
                return;

            MainLog.Text += "Playing Track @" + seconds + " seconds\n";

            var audioFile = new AudioFileReader(songFilename);

            audioFile.Skip((int)(seconds));
            outputDevice.Init(audioFile);

            timePick = seconds * 10;

            //string code = "@" + (timePick * 100) + "\r\n";
            DeviceManagerBLE.Play((ulong)timePick * 100);

            outputDevice.Play();
            outputDevice.Volume = volume/100.0f;

            literalTrackStartTime = DateTime.Now;

            playing = true;
        }

        private void StopTrack(bool resetLocation)
        {
            MainLog.Text += "Stopping track\n";

            outputDevice.Stop();
            //dispatcherTimer.Stop();
            playing = false;
            if (resetLocation)
            {
                Canvas.SetLeft(TrackIndexPlay, 0);
            }
            DeviceManagerBLE.SendMessage("(");
            DeviceManagerBLE.SendMessage("0");
        }

        private void WriteLogSlot(int slot, string title, string message, int color)
        {
            var brush = new SolidColorBrush(Color.FromRgb(127, 127, 127));
            if (color == 1)
                brush = new SolidColorBrush(Color.FromRgb(192,192, 63));
            if (color == 2)
                brush = new SolidColorBrush(Color.FromRgb(63, 192, 63));
            if (color == 3)
                brush = new SolidColorBrush(Color.FromRgb(192, 63, 63));

            switch (slot)
            {
                case 0:
                    Log1.Text = title + "\n" + message;
                    Log1.Foreground = brush;
                    Log1.ScrollToEnd();
                    break;
                case 1:
                    Log2.Text = title + "\n" + message;
                    Log2.Foreground = brush;
                    Log2.ScrollToEnd();
                    break;
                case 2:
                    Log3.Text = title + "\n" + message;
                    Log3.Foreground = brush;
                    Log3.ScrollToEnd();
                    break;
                case 3:
                    Log4.Foreground = brush;
                    Log4.Text = title + "\n" + message;
                    Log4.ScrollToEnd();
                    break;
                case 4:
                    Log5.Text = title + "\n" + message;
                    Log5.Foreground = brush;
                    Log5.ScrollToEnd();
                    break;
                case 5:
                    Log6.Text = title + "\n" + message;
                    Log6.Foreground = brush;
                    Log6.ScrollToEnd();
                    break;
                case 6:
                    Log7.Text = title + "\n" + message;
                    Log7.Foreground = brush;
                    Log7.ScrollToEnd();
                    break;
                case 7:
                    Log8.Foreground = brush;
                    Log8.Text = title + "\n" + message;
                    Log8.ScrollToEnd();
                    break;
            }
        }

        private void StopAndSendToJoy(int joyId, string value)
        {
            if (joyId == 0 && JoystickToBle1 != "")
                DeviceManagerBLE.SendMessage(JoystickToBle1, value);
            if (joyId == 1 && JoystickToBle2 != "")
                DeviceManagerBLE.SendMessage(JoystickToBle2, value);
        }
        private void ProcessControllerEvent(int joyId, GameControllerEvent joyEvent)
        {

            switch (joyEvent)
            {
                case GameControllerEvent.B1_Dn: StopAndSendToJoy(joyId, "C"); break;
                case GameControllerEvent.B1_Up: break;
                case GameControllerEvent.B2_Dn: StopAndSendToJoy(joyId, "M"); break;
                case GameControllerEvent.B2_Up: break;
                case GameControllerEvent.B3_Dn: StopAndSendToJoy(joyId, "B"); break;
                case GameControllerEvent.B3_Up: break;
                case GameControllerEvent.B4_Dn: StopAndSendToJoy(joyId, "N"); break;
                case GameControllerEvent.B4_Up: break;
                case GameControllerEvent.L1_Dn: StopAndSendToJoy(joyId, "!"); break;
                case GameControllerEvent.L1_Up: StopAndSendToJoy(joyId, "%"); break;
                case GameControllerEvent.L2_Dn: StopAndSendToJoy(joyId, "@"); break;
                case GameControllerEvent.L2_Up: StopAndSendToJoy(joyId, "^"); break;
                case GameControllerEvent.R1_Dn: StopAndSendToJoy(joyId, "#"); break;
                case GameControllerEvent.R1_Up: StopAndSendToJoy(joyId, "&"); break;
                case GameControllerEvent.R2_Dn: StopAndSendToJoy(joyId, "$"); break;
                case GameControllerEvent.R2_Up: StopAndSendToJoy(joyId, "*"); break;
                case GameControllerEvent.Select_Dn: StopAndSendToJoy(joyId, "Z"); break;
                case GameControllerEvent.Select_Up: break; 
                case GameControllerEvent.Start_Dn: //PlayTrack(0);break;
                case GameControllerEvent.Start_Up: break;
                case GameControllerEvent.J1_Dn: Console.WriteLine("J1_Dn"); break;
                case GameControllerEvent.J1_Up: Console.WriteLine("J1_Up"); break;
                case GameControllerEvent.J2_Dn: Console.WriteLine("J2_Dn"); break;
                case GameControllerEvent.J2_Up: Console.WriteLine("J2_Up"); break;
                case GameControllerEvent.XLeft_Dn: StopAndSendToJoy(joyId, "O"); break;
                case GameControllerEvent.XLeft_Up: break;
                case GameControllerEvent.XRight_Dn: StopAndSendToJoy(joyId, "P"); break;
                case GameControllerEvent.XRight_Up: break;
                case GameControllerEvent.YUp_Dn: StopAndSendToJoy(joyId, "Q"); break;
                case GameControllerEvent.YUp_Up: break;
                case GameControllerEvent.YDn_Dn: StopAndSendToJoy(joyId, "R"); break;
                case GameControllerEvent.YDn_Up: break;
            }
        }

        private void btVolumeTimer_Tick(object sender, EventArgs e)
        {
            float inputVolume = DeviceVolume.GetVolume();
            //float inputVolume = DeviceVolume.GetDirectVolume();

            if (inputVolume != lastInputVolume)
            {
                float TransVolume = (ulong)(inputVolume * 2.5);//convert 100 to 255
                TransVolume *= boostVolume;
                if (TransVolume > 255.0f)
                     TransVolume =255.0f;
                DeviceManagerBLE.Volume((byte)(TransVolume));
                lastInputVolume = inputVolume;
            }
        }

        private void btTextTimer_Tick(object sender, EventArgs e)
        {
            GameControllerEvent joyEvent1 = gameController.Poll(0);
            GameControllerEvent joyEvent2 = gameController.Poll(1);

            ProcessControllerEvent(0,joyEvent1);
            ProcessControllerEvent(1,joyEvent2);

            int slot = 0;
            foreach (KeyValuePair<string,BleDevice> kvp in DeviceManagerBLE.bleDevices)
            {
                int color = 0;
                if (DeviceManagerBLE.bleDevices[kvp.Key].log.Contains("Waiting"))
                    color = 1;
                if (DeviceManagerBLE.bleDevices[kvp.Key].log.Contains("OK"))
                    color = 2;
                if (DeviceManagerBLE.bleDevices[kvp.Key].log.Contains(" DC"))
                    color = 3;
                WriteLogSlot(slot, kvp.Value.name, DeviceManagerBLE.bleDevices[kvp.Key].log, color);
                slot++;
            }

            textTickCount++;

            MainLog.Text = "";
            float inputVolume = DeviceVolume.GetVolume();
            InputVolumeIndicator.Value = inputVolume*boostVolume;

            if (useSoundTrigger && inputVolume >= VolumeThreshold && !playing)
            {
                Canvas.SetLeft(TrackIndex, 0);
                timePick = 0;
                Canvas.SetLeft(TrackIndexPlay, timePick);

                PlayTrack(1);
            }

            if (playing)
                MainLog.Text += "Playing,";
            else MainLog.Text += "Stopped,";
            MainLog.Text += " Seek " + (timePick*100.0f)+ " ms\n";
            MainLog.Text += "SoundTrigger " + (useSoundTrigger ? "ON" : "off") + "\n";
            MainLog.Text += "SoundInput " + DeviceVolume.GetName() + "\n";

            if (DeviceVolume.IsActive())
                MainLog.Text += "Input (" + (int)(DeviceVolume.GetBias()*100.0f) + "%) "+ (int)inputVolume + " of " + (int)VolumeThreshold + "\n";
            else MainLog.Text += "Input inactive\n";

            if (foundDevices != null)
                MainLog.Text += "Devices " + foundDevices.Count + "\n";

            
            /* MainLog.Text += "Actives:\n";
             foreach (KeyValuePair<string, BleDevice> kvp in DeviceManagerBLE.bleDevices)
                 MainLog.Text += " " + kvp.Value.log + "\n";

             foreach (KeyValuePair<string, BluetoothClient> kvp in DeviceManagerBT.clients)
             {
                 if (kvp.Value.Connected)
                     MainLog.Text += " (BT) " + kvp.Key + " ok\n";
                 else if (kvp.Value.Connected)
                     MainLog.Text += " (BT) " + kvp.Key + " NOT ok\n";
             }*/

            string bleScanned = "";
            if (foundDevices!= null)
            {
                bleScanned = "Found: " + foundDevices.Count + "\n";
                foreach (KeyValuePair<string, string> kvp in foundDevices)
                {
                    bleScanned += kvp.Value + " : " + kvp.Key + "\n";
                }
            }
            MainLog.Text += bleScanned;


            //MainLog.Text += "\n";
            MainLog.Text += DeviceManagerBLE.MonitorLog;
            MainLog.ScrollToEnd();
        }

        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            if (!playing) return;

            if (outputDevice.PlaybackState == PlaybackState.Stopped)
                StopTrack(true);

            //4,416,049
            TimeSpan timeElapsedSincePlay = DateTime.Now - literalTrackStartTime;// + new TimeSpan(0,0,0,0,(int)timePlay*100);
            TimeSpan timeCurrentInSong = timeElapsedSincePlay + new TimeSpan(0, 0, 0, 0, (int)timePick * 100);

            double timeValue = (double)(timeCurrentInSong.TotalMilliseconds) / 100;
            Canvas.SetLeft(TrackIndexPlay, Math.Round(timeValue, 0));
            CommandManager.InvalidateRequerySuggested();
        }

        private void Exit(object sender, RoutedEventArgs e)
        {
            DeviceManagerBLE.DisconnectAll();
            MainLog.Text += "Exiting\n";
            System.Windows.Application.Current.Shutdown();
            Environment.Exit(0);
        }

        private static void DiscoverBleThread(object in_name)
        {
            foundDevices = DeviceSearchBLE.DiscoverDevicesAsync().Result;
        }

        private void BleSearchButton_Click(object sender, RoutedEventArgs e)
        {
            discoverBleThread = new Thread(DiscoverBleThread);
            discoverBleThread.Start();
        }

        private void BleAuthButton_Click(object sender, RoutedEventArgs e)
        {
            DeviceManagerBLE.Auth(3838);
        }

        private void PlayButton_Click(object sender, RoutedEventArgs e)
        {
            double ts = timePick;
            ts = Math.Round(ts / 10, 0);
            PlayTrack((int)ts);
        }
        private void StopButton_Click(object sender, RoutedEventArgs e)
        {
            Canvas.SetLeft(TrackIndexPlay, 0);
            StopTrack(true);
        }

        private void OnMouseDown(object sender, MouseEventArgs e)
        {
            if (playing)
                return;
            b1Down = true;
        }
        private void OnMouseUp(object sender, MouseEventArgs e)
        {
            b1Down = false;
        }

        private void OnMouseMove(object sender, MouseEventArgs e)
        {
            double Y = e.GetPosition(this).Y;
        }

        private void BoostVolumeSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            boostVolume = (float)e.NewValue / (float)50;
        }


        void StopAndSendToBoth(string value)
        {
            if (value == "gt") value = ">";
            else if (value == "lt") value = "<";
            DeviceManagerBLE.SendMessage(value);
        }
        private void SendBoth(object sender, RoutedEventArgs e)
        {
            var button = sender as Button;            
            StopAndSendToBoth(""+button.Tag);
        }

        private void SendKill(object sender, RoutedEventArgs e)
        {
            var button = sender as Button;
            StopAndSendToBoth("" + button.Tag);

            for (int i = 1; i < 100; i++)
            {
                System.Windows.Controls.Button fx = (System.Windows.Controls.Button)(MainCanvas.FindName("FX" + i));
                if (fx != null)
                    fx.Background = new SolidColorBrush(Color.FromRgb(0X12, 0X24, 0x61));
            }
        }
        private void SendColor(object sender, RoutedEventArgs e)
        {
            var button = sender as Button;
            string buttonTag = button.Tag.ToString();
            char buttonTagChar = buttonTag[0];

            switch (buttonTagChar)
            {
                case '0': StopAndSendToBoth("0"); break;
                case '1': StopAndSendToBoth("1"); break;
                case '2': StopAndSendToBoth("2"); break;
                case '3': StopAndSendToBoth("3"); break;
                case '4': StopAndSendToBoth("4"); break;
                case '5': StopAndSendToBoth("5"); break;
                case '6': StopAndSendToBoth("6"); break;
                case '7': StopAndSendToBoth("7"); break;
                case '8': StopAndSendToBoth("8"); break;
                case '9': StopAndSendToBoth("9"); break;
            }
        }

        private void InputVolumeTriggerSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            VolumeThreshold = (int)e.NewValue;
        }

        private void InputVolumeSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            DeviceVolume.SetBias((float)e.NewValue/50.0f);//100 * 2
        }

        private void TriggerCheckBox_Checked(object sender, RoutedEventArgs e)
        {
            useSoundTrigger = true;
        }

        private void TriggerCheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            useSoundTrigger = false;
        }

        private void ToggleOn(object sender, ulong channel)
        {
            ((System.Windows.Controls.Button)sender).Background = new SolidColorBrush(Color.FromRgb(0, 0x48, 0));
            DeviceManagerBLE.ToggleOn(channel);
        }
        private void ToggleOff(object sender, ulong channel)
        {
            ((System.Windows.Controls.Button)sender).Background = new SolidColorBrush(Color.FromRgb(0X12, 0X24, 0x61));
            DeviceManagerBLE.ToggleOff(channel);
        }
        private void Toggle(object sender, bool on, ulong channel)
        {
            if (on) ToggleOn(sender, channel);
            else ToggleOff(sender, channel);
        }

        private void SendToggle(object sender, RoutedEventArgs e)
        {
            var button = sender as Button;
            string buttonTag = button.Tag.ToString();
            char buttonTagChar = buttonTag[0];
            Color color = ((SolidColorBrush)(((System.Windows.Controls.Button)sender).Background)).Color;
            bool isOff = color.B > 0;
            ulong channel = (ulong)(int)buttonTagChar - (int)'0';
            Toggle(sender, isOff, channel);
        }

        private void SendExcite(object sender, RoutedEventArgs e)
        {
            var button = sender as Button;
            string buttonTag = button.Tag.ToString();
            char buttonTagChar = buttonTag[0];
            ulong channel = (ulong)(int)buttonTagChar - (int)'0';
            DeviceManagerBLE.Excite(channel);
        }

        private void SendReset(object sender, RoutedEventArgs e)
        {
            var button = sender as Button;
            string buttonTag = button.Tag.ToString();
            char buttonTagChar = buttonTag[0];
            ulong channel = (ulong)(int)buttonTagChar - (int)'0';
            DeviceManagerBLE.Reset(channel);
        }

        private void SendPal(object sender, RoutedEventArgs e)
        {
            var button = sender as Button;
            ulong paletteId = Convert.ToUInt64(button.Tag.ToString());
            DeviceManagerBLE.Palette(paletteId);
        }

        private void SendSideFXPreset(object sender, RoutedEventArgs e)
        {
            var button = sender as Button;
            ulong paletteId = Convert.ToUInt64(button.Tag.ToString());
            DeviceManagerBLE.Palette(paletteId);
        }

        private void OnButtonKeyUp(object sender, KeyEventArgs e)
        {
            switch (e.Key)
            {
                case Key.D1: DeviceManagerBLE.Toggle(1); break;
                case Key.D2: DeviceManagerBLE.Toggle(2); break;
                case Key.D3: DeviceManagerBLE.Toggle(3); break;
                case Key.D4: DeviceManagerBLE.Toggle(4); break;
                case Key.D5: DeviceManagerBLE.Toggle(5); break;
                case Key.D6: DeviceManagerBLE.Toggle(6); break;
                case Key.D7: DeviceManagerBLE.Toggle(7); break;
                case Key.D8: DeviceManagerBLE.Toggle(8); break;
                case Key.D9: DeviceManagerBLE.Toggle(9); break;
                case Key.D0: StopAndSendToBoth("|");break;

                case Key.Q: DeviceManagerBLE.SideFXPreset(1); break;
                case Key.W: DeviceManagerBLE.SideFXPreset(2); break;
                case Key.E: DeviceManagerBLE.SideFXPreset(3); break;
                case Key.R: DeviceManagerBLE.SideFXPreset(4); break;
                case Key.T: DeviceManagerBLE.SideFXPreset(5); break;
                case Key.Y: DeviceManagerBLE.SideFXPreset(6); break;
                case Key.U: DeviceManagerBLE.SideFXPreset(7); break;
                case Key.I: DeviceManagerBLE.SideFXPreset(8); break;
                case Key.O: DeviceManagerBLE.SideFXPreset(9); break;
                case Key.P: DeviceManagerBLE.SideFXPreset(10); break;

                case Key.A: DeviceManagerBLE.Palette(150); break; //fx_palette_rbm
                case Key.S: DeviceManagerBLE.Palette(151); break; //fx_palette_rgb
                case Key.D: DeviceManagerBLE.Palette(152); break; //fx_palette_cmy
                case Key.F: DeviceManagerBLE.Palette(153); break; //fx_palette_cbm
                case Key.G: DeviceManagerBLE.Palette(205); break; //fx_palette_rainbow
                case Key.H: DeviceManagerBLE.Palette(206); break; //fx_palette_rainbowstripe
                case Key.J: DeviceManagerBLE.Palette(207); break; //fx_palette_party
                case Key.K: DeviceManagerBLE.Palette(208); break; //fx_palette_heat
                case Key.L: DeviceManagerBLE.Palette(209); break; //fx_palette_seahawks

                case Key.Z: StopAndSendToBoth("e"); break; //head
                case Key.X: StopAndSendToBoth("w"); break; //wings
                case Key.C: StopAndSendToBoth("q"); break; //torso
                case Key.V: StopAndSendToBoth("r"); break; //tail
                case Key.B: StopAndSendToBoth("o"); break; //all

                case Key.Oem1:  StopAndSendToBoth("-"); break;
                case Key.Oem7:  StopAndSendToBoth("="); break;
                case Key.Oem2:  StopAndSendToBoth("["); break;

            }
        }

    }
}
