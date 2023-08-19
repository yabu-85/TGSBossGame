#include "ObstacleManager.h"
#include "TestObstacle.h"
#include "WallObstacle.h"
#include "Engine/CsvReader.h"

ObstacleManager::ObstacleManager(GameObject* parent)
{
}

ObstacleManager::~ObstacleManager()
{
}

void ObstacleManager::Initialize()
{
#if 0    
    // CSV�t�@�C���ǂݍ���
    CsvReader csv;
    if (csv.Load("Map.csv")) 
    {
        // CSV�������ǂݍ���ŏ�Q���𐶐�
        for (int y = 0; y < csv.GetHeight(); ++y) 
        {
            std::string rowData = csv.GetString(y, 0); // 1�s���̃f�[�^���擾

            for (int x = 0; x < rowData.size(); ++x) 
            {
                if (rowData[x] == '1') 
                {
                    XMFLOAT3 position(static_cast<float>(x), static_cast<float>(y), 0); // Z���W��0�Ɖ���
                    createAndAddObstacle(position, ObstacleType::OBSTACLE_NORMAL);
                }
            }
        }
    }
#endif

#if 0
    // �t�B�[���h�𑖍����ď�Q���𐶐�
    for (int y = 0; y < obstacles_.size(); ++y) {
        for (int x = 0; x < obstacles_[y].size(); ++x) {
            if (obstacles_[y][x] == 1) {
                XMFLOAT3 position(static_cast<float>(x), static_cast<float>(y), 0);
                createAndAddObstacle(position, ObstacleType::OBSTACLE_NORMAL);
            }
        }
    }
#endif

    //////////csv����ʒu���Ăяo���\���������Ǐo���Ƃ�񡡡

    createAndAddObstacle(XMFLOAT3(0, 0, 0), OBSTACLE_NORMAL);
	createAndAddObstacle(XMFLOAT3(4, 1, 30), OBSTACLE_MISSILE);
	createAndAddObstacle(XMFLOAT3(2, 1, 30), OBSTACLE_WALL);
}

void ObstacleManager::Update()
{
}

void ObstacleManager::Draw()
{
}

void ObstacleManager::Release()
{
}

void ObstacleManager::addObstacle(Obstacle* _obstacle)
{
	obstacles_.push_back(_obstacle);
}

void ObstacleManager::createAndAddObstacle(XMFLOAT3 _position, ObstacleType _type)
{
    Obstacle* pObstacle = nullptr;

    switch (_type) 
    {
    case ObstacleType::OBSTACLE_NORMAL:
        pObstacle = Instantiate<Obstacle>(this);
        break;
    case ObstacleType::OBSTACLE_MISSILE:
        pObstacle = Instantiate<TestObstacle>(this);
        break;
    case ObstacleType::OBSTACLE_WALL:
        pObstacle = Instantiate<WallObstacle>(this);
        break;
    }

    //nullptr����Ȃ����
    if (pObstacle) 
    {
        pObstacle->SetPosition(_position);
        addObstacle(pObstacle);
    }

}
