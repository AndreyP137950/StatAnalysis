#pragma once

#include <windows.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Diagnostics;
using namespace System::Text;

namespace LabsGUIv20 {

    public ref class MyForm : public System::Windows::Forms::Form
    {
    public:
        MyForm(void)
        {
            this->SuspendLayout();
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
            InitializeComponent();
            this->ResumeLayout(false);
        }

    protected:
        ~MyForm()
        {
            if (components)
                delete components;
        }

    private:
        System::Windows::Forms::ToolStrip^ toolStrip1;
        System::Windows::Forms::ToolStripButton^ создатьToolStripButton;
        System::Windows::Forms::ToolStripButton^ открытьToolStripButton;
        System::Windows::Forms::ToolStripButton^ сохранитьToolStripButton;
        System::Windows::Forms::ToolStripButton^ печатьToolStripButton;
        System::Windows::Forms::ToolStripSeparator^ toolStripSeparator;
        System::Windows::Forms::ToolStripButton^ вырезатьToolStripButton;
        System::Windows::Forms::ToolStripButton^ копироватьToolStripButton;
        System::Windows::Forms::ToolStripButton^ вставкаToolStripButton;
        System::Windows::Forms::ToolStripSeparator^ toolStripSeparator1;
        System::Windows::Forms::ToolStripButton^ справкаToolStripButton;
        System::Windows::Forms::ToolStripButton^ ExpandAllNodes;
        System::Windows::Forms::ToolStripButton^ CollapseAllNodes;
        System::Windows::Forms::ToolStripButton^ LoadInputFile;
        System::Windows::Forms::ToolStripButton^ LoadOutputFile;
        System::Windows::Forms::ToolStripButton^ RunSelected;
        System::Windows::Forms::ToolStripButton^ ClearRichbox;
        System::Windows::Forms::ToolStripButton^ PlotGraph;
        System::Windows::Forms::TreeView^ treeView1;
        System::Windows::Forms::RichTextBox^ BaseOutBox;
        System::ComponentModel::Container^ components;

        void InitializeComponent(void)
        {
            System::Windows::Forms::TreeNode^ treeNode1 = (gcnew System::Windows::Forms::TreeNode(L"Нормальное распределение"));
            System::Windows::Forms::TreeNode^ treeNode2 = (gcnew System::Windows::Forms::TreeNode(L"Распределение Вейбулла"));
            System::Windows::Forms::TreeNode^ treeNode3 = (gcnew System::Windows::Forms::TreeNode(L"Метод максимального правдоподобия",
                gcnew cli::array< System::Windows::Forms::TreeNode^  >(2) { treeNode1, treeNode2 }));
            System::Windows::Forms::TreeNode^ treeNode4 = (gcnew System::Windows::Forms::TreeNode(L"Нормальное распределение"));
            System::Windows::Forms::TreeNode^ treeNode5 = (gcnew System::Windows::Forms::TreeNode(L"Распределение Вейбулла"));
            System::Windows::Forms::TreeNode^ treeNode6 = (gcnew System::Windows::Forms::TreeNode(L"Метод наименьших квадратов",
                gcnew cli::array< System::Windows::Forms::TreeNode^  >(2) { treeNode4, treeNode5 }));
            System::Windows::Forms::TreeNode^ treeNode7 = (gcnew System::Windows::Forms::TreeNode(L"Параметрические методы",
                gcnew cli::array< System::Windows::Forms::TreeNode^  >(2) { treeNode3, treeNode6 }));
            System::Windows::Forms::TreeNode^ treeNode8 = (gcnew System::Windows::Forms::TreeNode(L"Узел6"));
            System::Windows::Forms::TreeNode^ treeNode9 = (gcnew System::Windows::Forms::TreeNode(L"Метод \"Up-Down\"",
                gcnew cli::array< System::Windows::Forms::TreeNode^  >(1) { treeNode8 }));
            System::Windows::Forms::TreeNode^ treeNode10 = (gcnew System::Windows::Forms::TreeNode(L"Регрессионный анализ"));
            System::Windows::Forms::TreeNode^ treeNode11 = (gcnew System::Windows::Forms::TreeNode(L"Статистическое оценивание",
                gcnew cli::array< System::Windows::Forms::TreeNode^  >(3) { treeNode7, treeNode9, treeNode10 }));
            System::Windows::Forms::TreeNode^ treeNode12 = (gcnew System::Windows::Forms::TreeNode(L"Узел15"));
            System::Windows::Forms::TreeNode^ treeNode13 = (gcnew System::Windows::Forms::TreeNode(L"Планирование испытаний",
                gcnew cli::array< System::Windows::Forms::TreeNode^  >(1) { treeNode12 }));
            System::Windows::Forms::TreeNode^ treeNode14 = (gcnew System::Windows::Forms::TreeNode(L"Критерий Граббса"));
            System::Windows::Forms::TreeNode^ treeNode15 = (gcnew System::Windows::Forms::TreeNode(L"Критерии аномальности",
                gcnew cli::array< System::Windows::Forms::TreeNode^  >(1) { treeNode14 }));
            System::Windows::Forms::TreeNode^ treeNode16 = (gcnew System::Windows::Forms::TreeNode(L"Критерий Стьюдента"));
            System::Windows::Forms::TreeNode^ treeNode17 = (gcnew System::Windows::Forms::TreeNode(L"Критерий Фишера"));
            System::Windows::Forms::TreeNode^ treeNode18 = (gcnew System::Windows::Forms::TreeNode(L"Однофакторный дисперсионный анализ (ANOVA)"));
            System::Windows::Forms::TreeNode^ treeNode19 = (gcnew System::Windows::Forms::TreeNode(L"Критерий Барлетта"));
            System::Windows::Forms::TreeNode^ treeNode20 = (gcnew System::Windows::Forms::TreeNode(L"Критерии однородности",
                gcnew cli::array< System::Windows::Forms::TreeNode^  >(4) { treeNode16, treeNode17, treeNode18, treeNode19 }));
            System::Windows::Forms::TreeNode^ treeNode21 = (gcnew System::Windows::Forms::TreeNode(L"Критерий хи-квадрат"));
            System::Windows::Forms::TreeNode^ treeNode22 = (gcnew System::Windows::Forms::TreeNode(L"Критерий Шапиро-Уилка"));
            System::Windows::Forms::TreeNode^ treeNode23 = (gcnew System::Windows::Forms::TreeNode(L"Узел23"));
            System::Windows::Forms::TreeNode^ treeNode24 = (gcnew System::Windows::Forms::TreeNode(L"Узел24"));
            System::Windows::Forms::TreeNode^ treeNode25 = (gcnew System::Windows::Forms::TreeNode(L"Узел25"));
            System::Windows::Forms::TreeNode^ treeNode26 = (gcnew System::Windows::Forms::TreeNode(L"Критерии согласия",
                gcnew cli::array< System::Windows::Forms::TreeNode^  >(5) { treeNode21, treeNode22, treeNode23, treeNode24, treeNode25 }));
            System::Windows::Forms::TreeNode^ treeNode27 = (gcnew System::Windows::Forms::TreeNode(L"Параметрические критерии",
                gcnew cli::array< System::Windows::Forms::TreeNode^  >(3) { treeNode15, treeNode20, treeNode26 }));
            System::Windows::Forms::TreeNode^ treeNode28 = (gcnew System::Windows::Forms::TreeNode(L"Критерий Уилкоксона"));
            System::Windows::Forms::TreeNode^ treeNode29 = (gcnew System::Windows::Forms::TreeNode(L"Узел26"));
            System::Windows::Forms::TreeNode^ treeNode30 = (gcnew System::Windows::Forms::TreeNode(L"Узел27"));
            System::Windows::Forms::TreeNode^ treeNode31 = (gcnew System::Windows::Forms::TreeNode(L"Узел28"));
            System::Windows::Forms::TreeNode^ treeNode32 = (gcnew System::Windows::Forms::TreeNode(L"Непараметрические критерии",
                gcnew cli::array< System::Windows::Forms::TreeNode^  >(4) { treeNode28, treeNode29, treeNode30, treeNode31 }));
            System::Windows::Forms::TreeNode^ treeNode33 = (gcnew System::Windows::Forms::TreeNode(L"Проверка статистических гипотез",
                gcnew cli::array< System::Windows::Forms::TreeNode^  >(2) { treeNode27, treeNode32 }));
            System::Windows::Forms::TreeNode^ treeNode34 = (gcnew System::Windows::Forms::TreeNode(L"Статистический анализ",
                gcnew cli::array< System::Windows::Forms::TreeNode^  >(3) { treeNode11, treeNode13, treeNode33 }));

            this->components = (gcnew System::ComponentModel::Container());
            this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
            this->создатьToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
            this->открытьToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
            this->сохранитьToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
            this->печатьToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripSeparator = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->вырезатьToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
            this->копироватьToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
            this->вставкаToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->справкаToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
            this->ExpandAllNodes = (gcnew System::Windows::Forms::ToolStripButton());
            this->CollapseAllNodes = (gcnew System::Windows::Forms::ToolStripButton());
            this->LoadInputFile = (gcnew System::Windows::Forms::ToolStripButton());
            this->LoadOutputFile = (gcnew System::Windows::Forms::ToolStripButton());
            this->RunSelected = (gcnew System::Windows::Forms::ToolStripButton());
            this->ClearRichbox = (gcnew System::Windows::Forms::ToolStripButton());
            this->PlotGraph = (gcnew System::Windows::Forms::ToolStripButton());
            this->treeView1 = (gcnew System::Windows::Forms::TreeView());
            this->BaseOutBox = (gcnew System::Windows::Forms::RichTextBox());
            this->toolStrip1->SuspendLayout();
            this->SuspendLayout();

            // toolStrip1
            this->toolStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
            this->toolStrip1->Items->AddRange(
                gcnew cli::array< System::Windows::Forms::ToolStripItem^ >(17) {
                this->создатьToolStripButton,
                    this->открытьToolStripButton,
                    this->сохранитьToolStripButton,
                    this->печатьToolStripButton,
                    this->toolStripSeparator,
                    this->вырезатьToolStripButton,
                    this->копироватьToolStripButton,
                    this->вставкаToolStripButton,
                    this->toolStripSeparator1,
                    this->справкаToolStripButton,
                    this->ExpandAllNodes,
                    this->CollapseAllNodes,
                    this->LoadInputFile,
                    this->LoadOutputFile,
                    this->RunSelected,
                    this->ClearRichbox,
                    this->PlotGraph
            });
            this->toolStrip1->Location = System::Drawing::Point(0, 0);
            this->toolStrip1->Name = L"toolStrip1";
            this->toolStrip1->Size = System::Drawing::Size(1317, 27);
            this->toolStrip1->TabIndex = 0;

            // кнопки тулбара (иконки можно не задавать)
            this->создатьToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->создатьToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->создатьToolStripButton->Name = L"создатьToolStripButton";
            this->создатьToolStripButton->Size = System::Drawing::Size(29, 24);
            this->создатьToolStripButton->Text = L"&Создать";

            this->открытьToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->открытьToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->открытьToolStripButton->Name = L"открытьToolStripButton";
            this->открытьToolStripButton->Size = System::Drawing::Size(29, 24);
            this->открытьToolStripButton->Text = L"&Открыть";

            this->сохранитьToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->сохранитьToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->сохранитьToolStripButton->Name = L"сохранитьToolStripButton";
            this->сохранитьToolStripButton->Size = System::Drawing::Size(29, 24);
            this->сохранитьToolStripButton->Text = L"&Сохранить";

            this->печатьToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->печатьToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->печатьToolStripButton->Name = L"печатьToolStripButton";
            this->печатьToolStripButton->Size = System::Drawing::Size(29, 24);
            this->печатьToolStripButton->Text = L"&Печать";

            this->toolStripSeparator->Name = L"toolStripSeparator";
            this->toolStripSeparator->Size = System::Drawing::Size(6, 27);

            this->вырезатьToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->вырезатьToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->вырезатьToolStripButton->Name = L"вырезатьToolStripButton";
            this->вырезатьToolStripButton->Size = System::Drawing::Size(29, 24);
            this->вырезатьToolStripButton->Text = L"В&ырезать";

            this->копироватьToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->копироватьToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->копироватьToolStripButton->Name = L"копироватьToolStripButton";
            this->копироватьToolStripButton->Size = System::Drawing::Size(29, 24);
            this->копироватьToolStripButton->Text = L"&Копировать";

            this->вставкаToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->вставкаToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->вставкаToolStripButton->Name = L"вставкаToolStripButton";
            this->вставкаToolStripButton->Size = System::Drawing::Size(29, 24);
            this->вставкаToolStripButton->Text = L"Вст&авка";

            this->toolStripSeparator1->Name = L"toolStripSeparator1";
            this->toolStripSeparator1->Size = System::Drawing::Size(6, 27);

            this->справкаToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->справкаToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->справкаToolStripButton->Name = L"справкаToolStripButton";
            this->справкаToolStripButton->Size = System::Drawing::Size(29, 24);
            this->справкаToolStripButton->Text = L"Спр&авка";

            this->ExpandAllNodes->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->ExpandAllNodes->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->ExpandAllNodes->Name = L"ExpandAllNodes";
            this->ExpandAllNodes->Size = System::Drawing::Size(123, 24);
            this->ExpandAllNodes->Text = L"ExpandAllNodes";
            this->ExpandAllNodes->Click += gcnew System::EventHandler(this, &MyForm::ExpandAllNodes_Click);

            this->CollapseAllNodes->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->CollapseAllNodes->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->CollapseAllNodes->Name = L"CollapseAllNodes";
            this->CollapseAllNodes->Size = System::Drawing::Size(131, 24);
            this->CollapseAllNodes->Text = L"CollapseAllNodes";
            this->CollapseAllNodes->Click += gcnew System::EventHandler(this, &MyForm::CollapseAllNodes_Click);

            this->LoadInputFile->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->LoadInputFile->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->LoadInputFile->Name = L"LoadInputFile";
            this->LoadInputFile->Size = System::Drawing::Size(67, 24);
            this->LoadInputFile->Text = L"LoadInp";
            this->LoadInputFile->Click += gcnew System::EventHandler(this, &MyForm::LoadInputFile_Click);

            this->LoadOutputFile->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->LoadOutputFile->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->LoadOutputFile->Name = L"LoadOutputFile";
            this->LoadOutputFile->Size = System::Drawing::Size(70, 24);
            this->LoadOutputFile->Text = L"LoadOut";
            this->LoadOutputFile->Click += gcnew System::EventHandler(this, &MyForm::LoadOutputFile_Click);

            this->RunSelected->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->RunSelected->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->RunSelected->Name = L"RunSelected";
            this->RunSelected->Size = System::Drawing::Size(38, 24);
            this->RunSelected->Text = L"Run";
            this->RunSelected->Click += gcnew System::EventHandler(this, &MyForm::RunSelected_Click);

            this->ClearRichbox->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->ClearRichbox->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->ClearRichbox->Name = L"ClearRichbox";
            this->ClearRichbox->Size = System::Drawing::Size(74, 24);
            this->ClearRichbox->Text = L"ClearText";
            this->ClearRichbox->Click += gcnew System::EventHandler(this, &MyForm::ClearRichbox_Click);

            this->PlotGraph->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->PlotGraph->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->PlotGraph->Name = L"PlotGraph";
            this->PlotGraph->Size = System::Drawing::Size(79, 24);
            this->PlotGraph->Text = L"PlotGraph";
            this->PlotGraph->Click += gcnew System::EventHandler(this, &MyForm::PlotGraph_Click);

            // treeView1: проставляем Tag, чтобы связать с проектами
            this->treeView1->Location = System::Drawing::Point(12, 58);
            this->treeView1->Name = L"treeView1";

            treeNode1->Name = L"NodeMLE_Normal";
            treeNode1->Tag = L"MLE_Normal";
            treeNode1->Text = L"Нормальное распределение";

            treeNode2->Name = L"NodeMLE_Weibull";
            treeNode2->Tag = L"MLE_Weibull";
            treeNode2->Text = L"Распределение Вейбулла";

            treeNode3->Name = L"NodeMLE";
            treeNode3->Text = L"Метод максимального правдоподобия";

            treeNode4->Name = L"NodeMLS_Normal";
            treeNode4->Tag = L"MLS_Normal";
            treeNode4->Text = L"Нормальное распределение";

            treeNode5->Name = L"NodeMLS_Weibull";
            treeNode5->Tag = L"MLS_Weibull";
            treeNode5->Text = L"Распределение Вейбулла";

            treeNode6->Name = L"NodeMLS";
            treeNode6->Text = L"Метод наименьших квадратов";

            treeNode14->Name = L"NodeGrubbs";
            treeNode14->Tag = L"Test_Grubbs";
            treeNode14->Text = L"Критерий Граббса";

            treeNode16->Name = L"NodeStudent";
            treeNode16->Tag = L"Test_Student";
            treeNode16->Text = L"Критерий Стьюдента";

            treeNode17->Name = L"NodeFisher";
            treeNode17->Tag = L"Test_Fisher";
            treeNode17->Text = L"Критерий Фишера";

            treeNode18->Name = L"NodeANOVA";
            treeNode18->Tag = L"Test_ANOVA";
            treeNode18->Text = L"Однофакторный дисперсионный анализ (ANOVA)";

            treeNode22->Name = L"NodeShapiroWilk";
            treeNode22->Tag = L"Test_ShapiroWilk";
            treeNode22->Text = L"Критерий Шапиро-Уилка";

            treeNode28->Name = L"NodeWilcoxon";
            treeNode28->Tag = L"Test_Wilcoxon";
            treeNode28->Text = L"Критерий Уилкоксона";

            treeNode34->Name = L"MainNode";
            treeNode34->Text = L"Статистический анализ";

            this->treeView1->Nodes->AddRange(
                gcnew cli::array< System::Windows::Forms::TreeNode^ >(1) { treeNode34 });
            this->treeView1->Size = System::Drawing::Size(477, 377);
            this->treeView1->TabIndex = 1;

            // BaseOutBox
            this->BaseOutBox->Location = System::Drawing::Point(548, 58);
            this->BaseOutBox->Name = L"BaseOutBox";
            this->BaseOutBox->Size = System::Drawing::Size(757, 377);
            this->BaseOutBox->TabIndex = 2;
            this->BaseOutBox->Text = L"";

            // MyForm
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1317, 493);
            this->Controls->Add(this->BaseOutBox);
            this->Controls->Add(this->treeView1);
            this->Controls->Add(this->toolStrip1);
            this->Name = L"MyForm";
            this->Text = L"Статистический анализ";
            this->toolStrip1->ResumeLayout(false);
            this->toolStrip1->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();
        }

    private:
        // имя базового файла (без расширения) по Tag
        System::String^ GetInpFileNameFromTag(System::String^ tag) {
            if (tag == "MLE_Normal") return "mle_normal";
            else if (tag == "MLE_Weibull") return "mle_weibull";
            else if (tag == "MLS_Normal") return "mls_normal";
            else if (tag == "MLS_Weibull") return "mls_weibull";
            else if (tag == "Test_Grubbs") return "grubbs";
            else if (tag == "Test_Student") return "studentfisher";     // строка 388: БЫЛО "test_student"
            else if (tag == "Test_Fisher") return "studentfisher";      // строка 389: БЫЛО "test_fisher"  
            else if (tag == "Test_ANOVA") return "annova";              // строка 390: БЫЛО "ANOVA"
            else if (tag == "Test_ShapiroWilk") return "shapirowilktest"; // строка 392: БЫЛО "ShapiroWilk"
            else if (tag == "Test_Wilcoxon") return "twosidedwilcoxon";  // строка 393: БЫЛО "Wilcoxon"
            return nullptr;
        }


        // относительная папка проекта (от final) по Tag
        System::String^ GetLabNumbFromTag(System::String^ tag) {
            // Laba1: MLE_Normal / MLE_Weibull → Laba1\Laba1
            if (tag == "MLE_Normal" || tag == "MLE_Weibull")
                return "Laba1\\\\Laba1";  // ← ДОЛЖНО быть именно так

            // Laba2: MLS_Normal / MLS_Weibull → Laba2\Laba2
            if (tag == "MLS_Normal" || tag == "MLS_Weibull")
                return "Laba2\\Laba2";

            // GrubbsTest: критерий Граббса
            if (tag == "Test_Grubbs")
                return "GrubbsTest\\GrubbsTest";

            // Student / Fisher — отдельный проект StudentFisherTest
            if (tag == "Test_Student" || tag == "Test_Fisher")
                return "StudentFisherTest\\StudentFisherTest";

            // ANOVA — свой проект ANOVA\ANOVA
            if (tag == "Test_ANOVA")
                return "ANOVA\\ANOVA";

            // Shapiro-Wilk и, при желании, Уилкоксон — проект ShapiroWilkTest
            if (tag == "Test_ShapiroWilk")
                return "ShapiroWilkTest\\ShapiroWilkTest";

            if (tag == "Test_Wilcoxon")
                return "Test_Wilcoxon\\Test_Wilcoxon";
            

            return nullptr;
        }

    private:
        System::Void ExpandAllNodes_Click(System::Object^, System::EventArgs^) {
            treeView1->BeginUpdate();
            treeView1->ExpandAll();
            treeView1->EndUpdate();
        }

        System::Void CollapseAllNodes_Click(System::Object^, System::EventArgs^) {
            treeView1->BeginUpdate();
            treeView1->CollapseAll();
            treeView1->EndUpdate();
        }

        System::Void LoadInputFile_Click(System::Object^, System::EventArgs^) {
            String^ baseRepoPath = L"D:\\MAI_2sem\\Data_analisys\\final";

            if (!treeView1 || !treeView1->SelectedNode || treeView1->SelectedNode->Tag == nullptr) {
                BaseOutBox->Text = L"Выберите узел";
                return;
            }

            String^ tag = treeView1->SelectedNode->Tag->ToString();
            String^ fileName = GetInpFileNameFromTag(tag);

            if (fileName == nullptr) {
                BaseOutBox->Text = L"Для выбранного узла не задан .inp (проверь Tag → имя файла).";
                return;
            }

            String^ labRel = GetLabNumbFromTag(tag);
            if (labRel == nullptr) {
                BaseOutBox->Text = L"Не удаётся определить папку проекта по Tag.";
            }

            String^ workingDir = Path::Combine(baseRepoPath, labRel);
            String^ path = Path::Combine(workingDir, "Inp");
            path = Path::Combine(path, fileName + ".inp");

            if (!File::Exists(path)) {
                BaseOutBox->Text = L"Файл не найден:\n" + path;
                return;
            }

            try {
                array<Encoding^>^ encodings = gcnew array<Encoding^>{
                    Encoding::UTF8,
                    Encoding::GetEncoding(1251),
                    Encoding::GetEncoding(866),
                    Encoding::Default
                };

                String^ content = nullptr;
                for each(Encoding ^ enc in encodings) {
                    auto sr = gcnew StreamReader(path, enc, true);
                    content = sr->ReadToEnd();
                    sr->Close();

                    bool hasGarbage = false;
                    for (int i = 0; i < Math::Min(100, content->Length); i++) {
                        wchar_t c = content[i];
                        if ((c >= 0x00C0 && c <= 0x00FF && c != 0x00AB && c != 0x00BB) ||
                            (c == 0xFFFD)) {
                            hasGarbage = true;
                            break;
                        }
                    }

                    if (!hasGarbage || enc == Encoding::Default)
                        break;
                }

                BaseOutBox->Clear();
                BaseOutBox->AppendText(content);
            }
            catch (Exception^ ex) {
                BaseOutBox->Text = L"Ошибка чтения файла:\n" + ex->Message;
            }
        }

        System::Void LoadOutputFile_Click(System::Object^, System::EventArgs^) {
            String^ baseRepoPath = L"D:\\MAI_2sem\\Data_analisys\\final";

            if (!treeView1 || !treeView1->SelectedNode || treeView1->SelectedNode->Tag == nullptr) {
                BaseOutBox->Text = L"Выберите узел";
                return;
            }

            String^ tag = treeView1->SelectedNode->Tag->ToString();
            String^ fileName = GetInpFileNameFromTag(tag);

            if (fileName == nullptr) {
                BaseOutBox->Text = L"Для выбранного узла не задан .out (проверь Tag → имя файла).";
                return;
            }

            String^ labRel = GetLabNumbFromTag(tag);
            if (labRel == nullptr) {
                BaseOutBox->Text = L"Не удаётся определить папку проекта по Tag.";
                return;
            }

            String^ workingDir = Path::Combine(baseRepoPath, labRel);
            String^ path = Path::Combine(workingDir, "Out");
            path = Path::Combine(path, fileName + ".out");

            if (!File::Exists(path)) {
                BaseOutBox->Text = L"Файл не найден:\n" + path;
                return;
            }

            try {
                array<Encoding^>^ encodings = gcnew array<Encoding^>{
                    Encoding::UTF8,
                    Encoding::GetEncoding(1251),
                    Encoding::GetEncoding(866),
                    Encoding::Default
                };

                String^ content = nullptr;
                for each(Encoding ^ enc in encodings) {
                    auto sr = gcnew StreamReader(path, enc, true);
                    content = sr->ReadToEnd();
                    sr->Close();

                    bool hasGarbage = false;
                    for (int i = 0; i < Math::Min(100, content->Length); i++) {
                        wchar_t c = content[i];
                        if ((c >= 0x00C0 && c <= 0x00FF && c != 0x00AB && c != 0x00BB) ||
                            (c == 0xFFFD)) {
                            hasGarbage = true;
                            break;
                        }
                    }

                    if (!hasGarbage || enc == Encoding::Default)
                        break;
                }

                BaseOutBox->Clear();
                BaseOutBox->AppendText(content);
            }
            catch (Exception^ ex) {
                BaseOutBox->Text = L"Ошибка чтения файла:\n" + ex->Message;
            }
        }

        System::Void RunSelected_Click(System::Object^, System::EventArgs^) {
            try {
                BaseOutBox->Clear();

                if (!treeView1 || !treeView1->SelectedNode || treeView1->SelectedNode->Tag == nullptr) {
                    BaseOutBox->Text = L"Выберите в дереве метод и распределение (узел с Tag).";
                    return;
                }

                String^ tag = treeView1->SelectedNode->Tag->ToString();
                String^ baseRepoPath = L"D:\\MAI_2sem\\Data_analisys\\final";

                String^ labRel = GetLabNumbFromTag(tag);
                if (labRel == nullptr) {
                    BaseOutBox->Text = L"[ERROR] Для выбранного узла не удалось определить проект (GetLabNumbFromTag).";
                    return;
                }

                String^ workingDir = Path::Combine(baseRepoPath, labRel);

                int lastSlash = labRel->LastIndexOf('\\');
                String^ projName = (lastSlash >= 0) ? labRel->Substring(lastSlash + 1) : labRel;

                String^ cfgFolder;
                String^ exeName;

                // ВСЕ Laba (MLE и MLS)
                if (tag->StartsWith("MLE_") || tag->StartsWith("MLS_")) {
                    bool isWeibull = tag->Contains("Weibull");
                    cfgFolder = isWeibull ? L"Debug-Weibull" : L"Debug-Normal";
                    exeName = projName + (isWeibull ? L"_Weibull.exe" : L"_Normal.exe");
                }
                // ВСЕ Test_*
                else if (tag->StartsWith("Test_")) {
                    cfgFolder = L"Debug";

                    if (tag == "Test_ANOVA") {
                        exeName = L"ANOVA.exe";
                        projName = L"ANOVA";
                    }
                    else if (tag == "Test_Grubbs") {
                        exeName = L"GrubbsTest.exe";
                        projName = L"GrubbsTest";
                    }
                    else if (tag == "Test_ShapiroWilk") {
                        exeName = L"ShapiroWilkTest.exe";
                        projName = L"ShapiroWilkTest";
                    }
                    else if (tag == "Test_Wilcoxon") {
                        exeName = L"ShapiroWilkTest.exe";
                        projName = L"ShapiroWilkTest";
                    }
                    else if (tag == "Test_Student" || tag == "Test_Fisher") {
                        exeName = L"StudentFisherTest.exe";
                        projName = L"StudentFisherTest";
                    }
                    else {
                        exeName = projName + L".exe";
                    }
                }
                else {
                    BaseOutBox->Text = L"[ERROR] Неизвестный тип узла: " + tag;
                    return;
                }

                String^ exePath = Path::Combine(baseRepoPath, projName);
                exePath = Path::Combine(exePath, "x64");
                exePath = Path::Combine(exePath, cfgFolder);
                exePath = Path::Combine(exePath, exeName);

                if (!Directory::Exists(workingDir)) {
                    BaseOutBox->Text = L"[ERROR] Рабочая папка проекта не найдена:\n" + workingDir;
                    return;
                }

                if (!File::Exists(exePath)) {
                    BaseOutBox->Text =
                        L"[ERROR] Не найден исполняемый файл:\n" + exePath +
                        L"\n\nПроверь, что выбрана и собрана конфигурация: " + cfgFolder +
                        L"\n(Сборка → Диспетчер конфигураций… для проекта " + projName + L")";
                    return;
                }

                auto psi = gcnew ProcessStartInfo(exePath);
                psi->UseShellExecute = false;
                psi->CreateNoWindow = true;
                psi->RedirectStandardOutput = true;
                psi->RedirectStandardError = true;
                psi->WorkingDirectory = workingDir;

                auto proc = gcnew Process();
                proc->StartInfo = psi;

                BaseOutBox->AppendText(L"[RUN]\n");
                BaseOutBox->AppendText(L" exe: " + exePath + L"\n");
                BaseOutBox->AppendText(L" cwd: " + workingDir + L"\n");
                BaseOutBox->AppendText(L" tag: " + tag + L"\n\n");

                if (!proc->Start()) {
                    BaseOutBox->AppendText(L"[ERROR] Не удалось запустить процесс.\n");
                    return;
                }

                String^ stdoutAll = proc->StandardOutput->ReadToEnd();
                String^ stderrAll = proc->StandardError->ReadToEnd();
                proc->WaitForExit();

                if (!String::IsNullOrEmpty(stdoutAll))
                    BaseOutBox->AppendText(L"[STDOUT]\n" + stdoutAll + L"\n");
                if (!String::IsNullOrEmpty(stderrAll))
                    BaseOutBox->AppendText(L"[STDERR]\n" + stderrAll + L"\n");

                BaseOutBox->AppendText(L"\n[ExitCode] " + proc->ExitCode + L"\n");

                String^ outBase = GetInpFileNameFromTag(tag);
                if (outBase != nullptr) {
                    String^ outDir = Path::Combine(workingDir, "Out");
                    String^ outPath = Path::Combine(outDir, outBase + ".out");
                    BaseOutBox->AppendText(
                        L"\nФайлы результата (если программа их создала):\n  " + outPath + L"\n");
                }
            }
            catch (System::Exception^ ex) {
                BaseOutBox->AppendText(L"\n[EXCEPTION] " + ex->Message + L"\n");
            }
        }

        System::Void ClearRichbox_Click(System::Object^, System::EventArgs^) {
            BaseOutBox->Clear();
        }

        System::Void PlotGraph_Click(System::Object^, System::EventArgs^) {
            String^ baseRepoPath = L"D:\\MAI_2sem\\Data_analisys\\final";

            if (!treeView1 || !treeView1->SelectedNode || treeView1->SelectedNode->Tag == nullptr) {
                BaseOutBox->Text = L"Выберите узел";
                return;
            }

            String^ tag = treeView1->SelectedNode->Tag->ToString();
            String^ labRel = GetLabNumbFromTag(tag);
            if (labRel == nullptr) {
                BaseOutBox->Text = L"Не удаётся определить папку проекта по Tag.";
                return;
            }

            String^ workingDir = Path::Combine(baseRepoPath, labRel);
            String^ path = Path::Combine(workingDir, "plot.xlsm");

            if (!File::Exists(path)) {
                BaseOutBox->Text = L"Файл не найден:\n" + path;
                return;
            }

            try {
                System::Diagnostics::ProcessStartInfo^ psi = gcnew System::Diagnostics::ProcessStartInfo();
                psi->FileName = path;
                psi->UseShellExecute = true;
                psi->WorkingDirectory = Path::GetDirectoryName(path);
                System::Diagnostics::Process::Start(psi);
            }
            catch (System::Exception^ ex) {
                System::Windows::Forms::MessageBox::Show(
                    this,
                    ex->ToString(),
                    L"Исключение при открытии plot.xlsm",
                    System::Windows::Forms::MessageBoxButtons::OK,
                    System::Windows::Forms::MessageBoxIcon::Error
                );
            }
        }
    };
}
