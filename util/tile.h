#pragma once

#include <string>
#include <vector>
#include <queue>
#include <cmath>

#include "base/files/file_path.h"
#include "base/logging.h"
#include "azer/render/render.h"
#include "azer/render/render_system_enum.h"

namespace azer {

class Frustrum;
class Tile {
 public:
  /**
   * tile �� grid line �����ļ��㹫ʽ�� 2 ^ level + 1
   * �������ַ����ĺô��ǿ��Էǳ����׵ļ����е�λ�� 2 ^ (level - 1) + 1
   */
  Tile(const int level = 8, float width = 1.0f)
      : kGridLine((1 << level) + 1)
      , kLevel_(level)
      , kCellWidth(width) {
  }

  struct Pitch {
    int left;
    int right;
    int top;
    int bottom;

    Pitch(): left(-1), right(-1), top(-1), bottom(-1) {}
    Pitch(int l, int t, int r, int b): left(l), right(r), top(t), bottom(b) {}
    Pitch(const Pitch& pitch) {*this = pitch; }
    Pitch& operator = (const Pitch& pitch);
  };

  void Init();
  const std::vector<azer::Vector3>& vertices() { return vertices_;}
  const std::vector<azer::Vector3>& normal() { return normal_;}
  const std::vector<azer::Vector2>& texcoord() { return texcoord_;}
  const std::vector<int32>& indices() { return indices_;}

  int32* InitPitchIndices(int level, const Tile::Pitch& pitch, int32* indices);

  /**
   * ע�⣺ �������������� x, z ���� grid line Ϊ���꣬����ʵ�ʵ� 3D ��������
   * 3D ���������� kCellWidth ���
   */
  void SetHeight(int x, int z, float height);
  azer::Vector3& vertex(int x, int z);
  const azer::Vector3& vertex(int x, int z) const;
  const azer::Vector2& texcoord(int x, int z) const;

  int GetGridLineNum() const { return kGridLine;}
  int GetVertexNum() const { return vertices_.size();}
  int GetIndicesNum() const { return indices_.size();}
  float cell_width() const { return kCellWidth;}
  void CalcNormal();
  /**
   * ���� TBN �ռ䣨��Ҫ�û� BUMPMAP)
   * ���� repeat: �������� texcoord, Ĭ������£�ÿһ����Ԫ��������� 1.0f
   * ͨ�� repeat �������Զ� texcoord ��������
   */
  void CalcTBN(float repeat, std::vector<azer::Vector3>* tangent,
               std::vector<azer::Vector3>* binormal);

  float minx() const;
  float maxx() const;
  float x_range() const { return maxx() - minx();}

  float miny() const;
  float maxy() const;
  float y_range() const { return maxy() - miny();}
  
  float minz() const;
  float maxz() const;
  float z_range() const { return maxz() - minz();}

  int level() const { return kLevel_;}

  class QuadTree {
   public:
    struct Node {
      Tile::Pitch pitch;
      int children[4];
      int level;
      bool splitted;
      Node(): level(-1), splitted(false) {}
      int midx() const { return (1 << (level - 1)) + pitch.left;}
      int midy() const { return (1 << (level - 1)) + pitch.top;}
    };

    explicit QuadTree(int level)
        : tail_(-1)
        , kLevel(level)
        , kGridLine((1 << level) + 1) {
      int max_cell = (int)std::pow(4.0, level) / 3;
      nodes_.reset(new Node[max_cell]);
    }

    class Splitable {
     public:
      enum SplitRes {
        kSplit,
        kKeep,
        kDrop,
      };
      virtual SplitRes Split(const Node& node) = 0;
    };

    void Split(Splitable* splitable, std::vector<Tile::Pitch>* nodes);
   private:
    void InitNode();
    void SplitPitch(Node* node);
    int tail_;
    std::unique_ptr<Node[]> nodes_;
    const int kLevel;
    const int kGridLine;
    DISALLOW_COPY_AND_ASSIGN(QuadTree);
  };
 private:
  void InitVertex();
  void InitIndices();
  std::vector<azer::Vector3> vertices_;
  std::vector<azer::Vector3> normal_;
  std::vector<azer::Vector2> texcoord_;
  std::vector<int32> indices_;
  const int kGridLine;
  const int kLevel_;
  const float kCellWidth;
  float min_x_, max_x_, min_y_, max_y_, min_z_, max_z_;
  bool yspec_;
  DISALLOW_COPY_AND_ASSIGN(Tile);
};

int32* InitPitchIndices(int level, const Tile::Pitch& pitch, int kGridLine,
                        int32* indices);

class FrustrumSplit : public Tile::QuadTree::Splitable {
 public:
  FrustrumSplit(const Tile* tile, const Frustrum* frustrum)
    : tile_(tile), frustrum_(frustrum) {}
  virtual SplitRes Split(const Tile::QuadTree::Node& node) override;
 private:
  const Tile* tile_;
  const Frustrum* frustrum_;
  DISALLOW_COPY_AND_ASSIGN(FrustrumSplit);
};

inline void Tile::QuadTree::InitNode() {
  tail_ = 1;
  Node& n = (nodes_.get())[0];
  n.splitted = false;
  n.pitch.left = 0;
  n.pitch.top = 0;
  n.pitch.right = kGridLine - 1;
  n.pitch.bottom = kGridLine - 1;
  n.level = kLevel;
}

inline Tile::Pitch& Tile::Pitch::operator = (const Pitch& pitch) {
  left = pitch.left;
  right = pitch.right;
  top = pitch.top;
  bottom = pitch.bottom;
  return *this;
}

inline float Tile::minx() const {
  DCHECK_GT(vertices_.size(), 0u);
  return min_x_;
}
inline float Tile::maxx() const {
  DCHECK_GT(vertices_.size(), 0u);
  return max_x_;
}
inline float Tile::miny() const {
  DCHECK(yspec_);
  DCHECK_GT(vertices_.size(), 0u);
  return min_y_;
}
inline float Tile::maxy() const {
  DCHECK(yspec_);
  DCHECK_GT(vertices_.size(), 0u);
  return max_y_;
}
inline float Tile::minz() const {
  DCHECK_GT(vertices_.size(), 0u);
  return min_z_;
}
inline float Tile::maxz() const {
  DCHECK_GT(vertices_.size(), 0u);
  return max_z_;
}

inline azer::Vector3& Tile::vertex(int x, int z) {
  return vertices_[z * kGridLine + x];
}

inline const azer::Vector3& Tile::vertex(int x, int z) const {
  return vertices_[z * kGridLine + x];
}
}  // namespace azer
