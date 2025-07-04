const xlsx = require('xlsx');
const fs = require('fs');
const path = require('path');

const EXCEL_DIR = './chapters';
const OUTPUT_DIR = './output';

// 캐릭터 설정
function buildCharacterSettings(row) {
    const settings = [];
    for (let i = 1; i <=3; i++) {
        const id = row[`characterId_${i}`];
        const exp = row[`expression_${i}`];
        const pos = row[`position_${i}`];

        if (id) {
            settings.push({
                characterId: id,
                expression: exp || 'Neutral',
                position: pos || 'Center'
            })
        }
    }

     return settings.length > 0 ? settings : [];
}

function parseJSONField(value) {
  if (!value) return [];
  try {
    return JSON.parse(value);
  } catch {
    return [];
  }
}


function convertWorkbook(filePath, chapterName) {
    const workbook = xlsx.readFile(filePath);
    const nodesSheet = xlsx.utils.sheet_to_json(workbook.Sheets['Nodes'] || {});
    const choicesSheet = xlsx.utils.sheet_to_json(workbook.Sheets['Choices'] || {});
    const conditionsSheet = xlsx.utils.sheet_to_json(workbook.Sheets['Conditions'] || {});
    const miniGameSheet = xlsx.utils.sheet_to_json(workbook.Sheets['MiniGame'] || {});
    const endSheet = xlsx.utils.sheet_to_json(workbook.Sheets['End'] || {});

    const result = {
        nodes: [],
        meta: {
            chapterName: chapterName
        }
    };

    // dialogue 시트 처리
    for (const row of nodesSheet) {
        const node = {
            id: row.nodeId,
            type: 'dialogue',
        };

        node.speakerId = row.speakerId;
        node.dialogue = row.dialogue;
        node.characterSettings = buildCharacterSettings(row);
        node.soundEffect = row.soundEffect || "None";
        node.BGImage = row.BGImage || "None";
        node.BGM = row.BGM || "None";
        node.cutScene = row.cutScene || "None";
        node.nextNodeId = row.nextNodeId || null;

        result.nodes.push(node);
    }

    // Choice 시트 처리
    for (const row of choicesSheet) {
        const node = {
            id: row.nodeId,
            type: 'choice',
            choices: [
                {
                    choiceText: row.choiceText,
                    requiredNodes: parseJSONField(row.requiredNodes),
                    requiredAffection: parseInt(row.requiredAffection) || 0,
                    relatedCharacterId: row.relatedCharacterId || null,
                    affectionGain: parseInt(row.affectionGain) || 0,
                    nextNodeId: row.nextNodeId
                }
            ]
        };

        // 만약 동일 nodeId가 이미 result.nodes에 있으면 거기에 선택지 추가해줘야함
        const existingNode = result.nodes.find(n => n.id === node.id);
        if (existingNode) {
            if (!existingNode.choices) existingNode.choices = [];
            existingNode.choices.push(...node.choices);
        } else {
            result.nodes.push(node);
        }
    }

    // Conditions 시트 처리
    for (const row of conditionsSheet) {
        const node = {
            id: row.nodeId,
            type: 'condition',
            requiredNodes: parseJSONField(row.requiredNodes),
            requiredAffection: parseInt(row.requiredAffection) || 0,
            relatedCharacterId: row.relatedCharacterId || null,
            trueNodeId: row.trueNodeId,
            falseNodeId: row.falseNodeId
        };

        // 중복 방지 및 merge
        const existingNode = result.nodes.find(n => n.id === node.id);
        if (existingNode) {
            Object.assign(existingNode, node);  // 기존 노드에 condition 필드 덮어쓰기
        } else {
            result.nodes.push(node);
        }
    }

    // MiniGame 시트 처리
    for (const row of miniGameSheet) {
        const node = {
            id: row.nodeId,
            type: 'miniGame',
            miniGameType: row.miniGameType,
            level: parseInt(row.level) || 0,
            nodeIdAfterClear: row.nodeIdAfterClear
        }

        // 중복 방지 및 merge
        const existingNode = result.nodes.find(n => n.id === node.id);
        if (existingNode) {
            Object.assign(existingNode, node);  // 기존 노드에 condition 필드 덮어쓰기
        } else {
            result.nodes.push(node);
        }
    }

    // end 시트 처리
    for (const row of endSheet) {
        const node = {
            id: row.nodeId,
            type: "end"
        }

        const existingNode = result.nodes.find(n => n.id == node.id);
        if (existingNode) {
            Object.assign(existingNode, node);
        } else {
            result.nodes.push(node);
        }

    }

    return result;
}


function runBatchConvert() {
  if (!fs.existsSync(OUTPUT_DIR)) fs.mkdirSync(OUTPUT_DIR);

  const files = fs.readdirSync(EXCEL_DIR).filter(file => file.endsWith('.xlsx'));

  files.forEach(file => {
    const chapterName = path.basename(file, '.xlsx');
    const inputPath = path.join(EXCEL_DIR, file);
    const outputPath = path.join(OUTPUT_DIR, `${chapterName}.json`);
    const data = convertWorkbook(inputPath, chapterName);

    fs.writeFileSync(outputPath, JSON.stringify(data, null, 2), 'utf-8');
    console.log(`✅ ${file} → ${chapterName}.json`);
  });
}

runBatchConvert();