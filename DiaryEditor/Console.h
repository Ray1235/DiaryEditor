#pragma once
class Console {
public:
	Console();
	~Console();
	char                  InputBuf[256];
	std::vector<char*>       Items;
	bool                  ScrollToBottom;
	std::vector<char*>       History;
	int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	std::vector<const char*> Commands;
	void    ClearLog();
	void    AddLog(const char* fmt, ...) IM_PRINTFARGS(2);
	void    Draw(const char* title, bool* p_open);
	void	ExecCommand(const char* command_line);

	static int TextEditCallbackStub(ImGuiTextEditCallbackData* data);
	int     TextEditCallback(ImGuiTextEditCallbackData* data);
};
extern Console console;
void ShowConsole(bool* p_open);