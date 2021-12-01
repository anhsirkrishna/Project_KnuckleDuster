#include "Animation.h"
#include "GLSprite.h"

Animation::Animation() : Component("ANIMATION"), p_owner_glsprite(NULL), current_frame(0), 
						 looping(true), interval(0), timer(0), times_played(0) {}

Animation::~Animation() {
	for (auto frame : frames) {
		delete frame;
	}
}

void Animation::Refresh() {
	current_frame = 0;
	times_played = 0;
	timer = 0;
}

void Animation::Update() {
	//If an entire loop as already been played then nothing else do to	
	if (current_frame == 0 && times_played > 0)
		return;

	if (frames.size() > 1) {
		timer += 1;
		if (timer == interval) {
			timer = 0;
			//Move to the next frame of animation 
			current_frame = current_frame + 1 < frames.size() ? (current_frame + 1) : 0;
		}
	}
	//Set the texture offset for the GLSprite component of the the owner
	p_owner_glsprite->SetTexOffset(frames[current_frame][0], frames[current_frame][1]);
}

void Animation::Serialize(json json_object) {
	auto tex_offset_list = json_object["frames"].get<std::vector<int>>();
	GLfloat* temp_offset;
	for (unsigned int i = 0; i < tex_offset_list.size(); i += 2) {
		temp_offset = new GLfloat[2];
		temp_offset[0] = tex_offset_list[i];
		temp_offset[1] = tex_offset_list[i + 1];
		frames.push_back(temp_offset);
	}

	looping = json_object["looping"].get<bool>();
	interval = json_object["interval"].get<int>();
}

void Animation::Link() {
	p_owner_glsprite = static_cast<GLSprite*>(GetOwner()->HasComponent("GLSPRITE"));
}