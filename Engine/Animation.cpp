#include "Components.h"

// FRAME START
Frame::Frame(SDL_Texture *texture, const SDL_Rect &source,
             const bool queryTexture)
    : m_texture(texture), m_src(source) {
  if (queryTexture) {
    SDL_QueryTexture(m_texture, nullptr, nullptr, &m_src.w, &m_src.h);
  }
}

void Frame::render(SDL_Renderer *renderer, const SDL_Rect &destination) {
  SDL_RenderCopy(renderer, m_texture, &m_src, &destination);
}

void Frame::setTexture(SDL_Texture *texture, const bool queryTexture) {
  m_texture = texture;
  if (queryTexture) {
    SDL_QueryTexture(m_texture, nullptr, nullptr, &m_src.w, &m_src.h);
  }
}

void Frame::setSource(const SDL_Rect &source, const bool queryTexture) {
  m_src = source;
  if (queryTexture) {
    SDL_QueryTexture(m_texture, nullptr, nullptr, &m_src.w, &m_src.h);
  }
}
// FRAME END

// ANIMATION FRAME START
AnimationFrame::AnimationFrame(const Frame &frame, const Uint32 ticks)
    : m_frame(frame), m_ticks(ticks) {}

AnimationFrame::AnimationFrame(SDL_Texture *texture, const SDL_Rect &source,
                               const Uint32 &ticks, const bool queryTexture)
    : AnimationFrame(Frame(texture, source, queryTexture), ticks) {}

void AnimationFrame::render(SDL_Renderer *renderer,
                            const SDL_Rect &destination) {
  m_frame.render(renderer, destination);
}
// ANIMATION FRAME END

// ANIMATION START
Animation::Animation(const std::vector<AnimationFrame> &animation)
    : m_frames(animation), m_nFrames(animation.size()) {}

void Animation::addFrame(const AnimationFrame &frame) {
  m_frames.push_back(frame);
  m_nFrames = m_frames.size();
}

void Animation::addFrames(const std::vector<AnimationFrame> &frames) {
  for (const auto &frame : frames) {
    addFrame(frame);
  }
}

void Animation::render(SDL_Renderer *renderer, const SDL_Rect &destination) {
  if (m_frames.empty()) {
    return;
  }
  m_frames[m_currFrame].render(renderer, destination);
}

void Animation::update(const Uint32 &dt) {
  m_currTicks += dt;
  if (m_frames.empty()) {
    return;
  }
  if (m_currTicks >= m_frames[m_currFrame].getTicks()) {
    m_currTicks %= m_frames[m_currFrame].getTicks();
    ++m_currFrame;
    if (m_currFrame == m_nFrames) {
      m_currFrame = 0;
    }
  }
}

void Animation::reset() {
  m_currTicks = 0;
  m_currFrame = 0;
}
// ANIMATION END
