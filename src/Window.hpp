#pragma once

namespace Window {
	void quit();
	void enableMouseWarp(bool enable);
	int mouseRelativeX();
	int mouseRelativeY();
	int mouseX();
	int mouseY();
	bool isMouseDown();
	bool wasMousePressed();
	bool wasMouseReleased();
}
