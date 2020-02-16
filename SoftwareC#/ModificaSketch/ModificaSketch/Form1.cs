using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Threading;

namespace ModificaSketch
{
    public partial class Form1 : Form
    {   /*
        TODO creare un sistema di riconoscimento del file da modificare per ora modifica solo quello che contiene anche le azioni
        */
        string[] sketchValidi = new string[6] { "INT_AirqualityControl", "SKT_TestTuttiSensori", "CalcoloR0SensoriMQ", "INT_AirqualityControl.ino", "SKT_TestTuttiSensori.ino", "CalcoloR0SensoriMQ.ino" };
        string[,] letturaVar = new string[11, 9];
        string percorsoFileConNome = ""; string nomeFile = "";
        int inizioMatrice = 23; int fineMatrice = 34; //23,34|15,20
        int nRighe = 11; int nColonne = 9;
        bool erroreLettura = false;
        public Form1()
        {
            InitializeComponent();
        }

        private void esciToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void apriToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialog1.Multiselect = false;
            openFileDialog1.ShowDialog();

            ApriFile(openFileDialog1.SafeFileName, openFileDialog1.FileName);
            CreaColonne();
            PopolaVettore();
            PopolaDataGrid();
        }
        private void ottieniToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        public void ApriFile(string _nome, string _percorso)
        {
            if(sketchValidi.Contains(_nome))
            {
                percorsoFileConNome = _percorso;
                nomeFile = _nome;
                this.Text = "ModificaSketch - modifica sketch " + nomeFile;
                //Assegno inizio e fine matrice per i diversi tipi di sketch
                if((nomeFile == "CalcoloR0SensoriMQ") || (nomeFile == "CalcoloR0SensoriMQ.ino"))
                {
                    inizioMatrice = 15;
                    fineMatrice = 20;
                    nRighe = 5;
                    nColonne = 9;
                }
                else
                {
                    inizioMatrice = 23;
                    fineMatrice = 34;
                    nRighe = 11;
                    nColonne = 9;
                }
            }
            else
                MessageBox.Show("Sketch non valido! Assicurati di caricare quello giusto.", "Attenzione", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
        }
        public void CreaColonne()
        {
            PulisciDataGrid();

            //Creazione colonne
            string[] nomeColonne = new string[11] { "Sensore", "MqInterno", "PinMqInterno", "RhInterno", "MqEsterno", "PinMqEsterno", "RhEsterno", "DeltaTmin20", "DeltaTmax20", "DeltaRh", "R0" };
            string[] colonnePrimaLetturaR0 = new string[5] { "Sensore", "Attivo", "PinMQInterno", "CostantiR0", "R0" };
            if ((nomeFile != "CalcoloR0SensoriMQ" && nomeFile != "CalcoloR0SensoriMQ.ino"))
            {
                for (int k = 0; k < nomeColonne.Length; k++)
                {
                    dataGridView1.Columns.Add(nomeColonne[k], nomeColonne[k]);
                }
            }
            else
            {
                for (int k = 0; k < colonnePrimaLetturaR0.Length; k++)
                {
                    dataGridView1.Columns.Add(colonnePrimaLetturaR0[k], colonnePrimaLetturaR0[k]);
                }
            }

        }
        public void PopolaVettore()
        {
            string line; int indexSorgente = 0; int indexRiga = 0;
            try
            {
                var file = new StreamReader(percorsoFileConNome);

                while ((line = file.ReadLine()) != null)
                {
                    if ((indexSorgente >= inizioMatrice) && (indexSorgente < fineMatrice))//righe del file in cui si trova la matrice da modificare
                    {
                        line = line.Remove(0, line.IndexOf('{') + 1);//elimino i primi elementi fino all'inizio della variabile
                        line = line.Remove(line.IndexOf('}'), line.Length - line.IndexOf('}'));//elimino gli elementi alla fine dell'ultima variabile
                        line = line.Replace("\"", "");//elimino le virgolette                    

                        string[] appo = line.Split(',');//splitto i valori
                        for (int k = 0; k < appo.Length; k++)
                        {   //tolgo lo spazio prima degli elementi
                            appo[k] = appo[k].Remove(0, 1);
                        }
                        //assegno i valori alla matrice
                        for (int j = 0; j < appo.Length; j++)
                        {
                            letturaVar[indexRiga, j] = appo[j];
                        }
                        indexRiga++;
                    }
                    indexSorgente++;
                }
                file.Close();
            }
            catch (Exception ecc)
            {
                MessageBox.Show(ecc.Message, "Errore", MessageBoxButtons.OK, MessageBoxIcon.Error);
                PulisciDataGrid();
                erroreLettura = true;
            }
        }

        public void PulisciDataGrid()
        {
            //Clear old Data
            dataGridView1.Rows.Clear();
            dataGridView1.Columns.Clear();
        }
        public void PopolaDataGrid()
        {
            //popola DataGrid al rovescio della matrice
            if (!erroreLettura)//controlla se la matrice è valida
            {
                for (int columnIndex = 0; columnIndex < /*letturaVar.GetLength(1)*/nColonne; ++columnIndex)//lunghezza colonna 9
                {
                    var row = new DataGridViewRow();

                    for (int rowIndex = 0; rowIndex < /*letturaVar.GetLength(0)*/nRighe; ++rowIndex)//lunghezza riga 11
                    {
                        if (rowIndex == 0)
                        {
                            row.Cells.Add(new DataGridViewTextBoxCell() { Value = letturaVar[rowIndex, columnIndex] });
                            row.Cells[0].ReadOnly = true;
                        }
                        else
                        {
                            if(nRighe!=5)//Se le righe sono diverse da 5 si parla dei file con le azioni o senza azioni
                            {
                                if ((rowIndex != 1) && (rowIndex != 4))
                                    row.Cells.Add(new DataGridViewTextBoxCell() { Value = letturaVar[rowIndex, columnIndex] });
                                else
                                    row.Cells.Add(new DataGridViewCheckBoxCell() { Value = Convert.ToBoolean(letturaVar[rowIndex, columnIndex]) });
                            }
                            else//Se le righe sono 5 si sta aprendo il file del calcolo di r0
                            {
                                if (rowIndex != 1)
                                    row.Cells.Add(new DataGridViewTextBoxCell() { Value = letturaVar[rowIndex, columnIndex] });
                                else
                                    row.Cells.Add(new DataGridViewCheckBoxCell() { Value = Convert.ToBoolean(letturaVar[rowIndex, columnIndex]) });
                            }

                        }
                    }
                    dataGridView1.Rows.Add(row);//Aggiunge la riga con i dati estrapolati dalla matrice
                }//fine popolamento datagrid
            }
            else
                erroreLettura = false;
        }

        public void TakeModifedCells()
        {
            //prelevare le modifiche e le assegno alla matrice globale
            for (int r = 0; r < /*11*/nRighe; r++)
            {
                for (int c = 0; c < /*9*/nColonne; c++)
                {
                    letturaVar[r, c] = Convert.ToString(dataGridView1.Rows[c].Cells[r].Value);
                }
            }

            //Ricostruzione righe
            string[] righeRicostruite = new string[11];
            for (int j = 0; j < /*11*/nRighe; j++)
            {
                for (int i = 0; i < /*9*/nColonne; i++)
                {
                    string elemento = letturaVar[j, i];
                    elemento = (i == 0) ? "  { \"" + elemento + "\"," : (i < 8) ? " \"" + elemento + "\"," : " \"" + elemento + "\"},";
                    righeRicostruite[j] += (j < 10) ? elemento : (i < 8) ? elemento : elemento.Substring(0, elemento.Length - 1);
                }
            }

            try
            {
                //lettura file d'origine
                string[] leggiFile = File.ReadAllLines(percorsoFileConNome);
                //leggere dalla index 23 al 33
                int indice = inizioMatrice; int s = 0;
                while (indice < fineMatrice)
                {
                    leggiFile[indice] = righeRicostruite[s];
                    s++;
                    indice++;
                }

                string directoryFile = percorsoFileConNome.Replace(nomeFile, "");
                File.Move(directoryFile + nomeFile, directoryFile + "OLD.ino");//rinomino il file d'origine
                var scriviFileModificato = new StreamWriter(percorsoFileConNome);

                for (int i = 0; i < leggiFile.Length; i++)
                {
                    scriviFileModificato.WriteLine(leggiFile[i]);
                }

                scriviFileModificato.Close();
                File.Delete(directoryFile + "OLD.ino");//elimina file origine
                MessageBox.Show("Modifiche effettuate con successo!", "Informazione", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ecc) { MessageBox.Show(ecc.Message, "Errore", MessageBoxButtons.OK, MessageBoxIcon.Error); };

            //Ricarico il file modificato
            PopolaVettore();
            CreaColonne();
            PopolaDataGrid();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.ShowDialog();
            LetturaCartellaSketch(folderBrowserDialog1.SelectedPath);
        }
        public void LetturaCartellaSketch(string _percorso)
        {
            comboBox1.Items.Clear();
            DirectoryInfo dirInfo;

            try
            {
                dirInfo = new DirectoryInfo(_percorso);                
                DirectoryInfo[] subDir = dirInfo.GetDirectories();
                textBox1.Text = _percorso;

                foreach (DirectoryInfo elm in subDir)
                {
                    if (sketchValidi.Contains(elm.Name))
                        comboBox1.Items.Add(elm.Name);
                }
                if (comboBox1.Items.Count != 0)
                {
                    comboBox1.SelectedIndex = 0;
                    MessageBox.Show("Sketch trovati!", "Informazione", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Sketch non trovati! Controlla se la cartella selezionata sia giusta, e contenga le cartelle di tutti gli sketch.", "Attenzione", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            catch (Exception ecc)
            {
                try
                {
                    dirInfo = new DirectoryInfo(_percorso);
                    ApriFile(dirInfo.Name, dirInfo.FullName);
                    PopolaVettore();
                    CreaColonne();
                    PopolaDataGrid();
                }
                catch { PulisciDataGrid(); }
            }

        }
        private void button2_Click(object sender, EventArgs e)
        {
            if (comboBox1.Items.Count != 0)
            {
                string nome = comboBox1.Items[comboBox1.SelectedIndex].ToString() + ".ino";
                string dirFullName = textBox1.Text + "\\" + comboBox1.Items[comboBox1.SelectedIndex].ToString() + "\\" + comboBox1.Items[comboBox1.SelectedIndex].ToString() + ".ino";
                ApriFile(nome, dirFullName);
                CreaColonne();
                PopolaVettore();
                PopolaDataGrid();
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            DialogResult risModifica = MessageBox.Show("Sicuro/a di voler salvare le modifiche effettuate al file " + nomeFile + " ?", "Salva modifiche", MessageBoxButtons.YesNo, MessageBoxIcon.Information);

            if (risModifica == DialogResult.Yes)
            {
                dataGridView1.CurrentCell = dataGridView1.Rows[0].Cells[0];
                TakeModifedCells();
            }
        }

        private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            string valBeforeEdit = letturaVar[e.ColumnIndex, e.RowIndex];
            string valAfterEdit = dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();

            if (valBeforeEdit != valAfterEdit)
                dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Style.BackColor = Color.Yellow;
            else
                dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Style.BackColor = Color.White;
        }

        private void dataGridView1_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effect = DragDropEffects.All;
            else
                e.Effect = DragDropEffects.None;
        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PulisciDataGrid();
        }

        private void dataGridView1_DragDrop(object sender, DragEventArgs e)
        {
            string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);           
            LetturaCartellaSketch(files[0]);
        }
    }
}